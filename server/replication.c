#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "replication.h"
#include "sp.h"
#include "file.h"
#include "mem_alloc.h"
#include "group_comm.h"
#include "handleclients.h"
#include "sync.h"
#include "debugmsg.h"

bool apply_create(struct update_record *rec_update);
bool apply_edit(struct update_record *rec_update);
bool apply_delete(struct update_record *rec_update);

void init_server(int server_id)
{
//    char extn_md[]=".db_data";
    char extn_log[]=".log";
//    int fd_md, 
    int fd_log;
    size_t read_bytes=0;
    size_t write_bytes=0;

    // Create the metadata file name of server
//    sprintf(meta_data_file,"%d_",server_id);
//    strcat(meta_data_file, extn_md);

    // Create the log file name of server
    sprintf(log_file,"%d_",server_id);
    strcat(log_file, extn_log);
    
//    Create/Open metadata file
//    fd_md = open(meta_data_file, O_RDWR | O_CREAT, S_IRWXU);
//    close(fd_md);
//    fp=fopen(meta_data_file, "r+");

    // Create/Open log file
    fd_log = open(log_file, O_RDWR | O_CREAT, S_IRWXU);
    close(fd_log);
    fp_log=fopen(log_file, "r+");

    printf("-->SSB: size of update record: %d\n", sizeof(struct update_record));
    printf("-->SSB: size of server info: %d\n", sizeof(struct server_info));
    // Initialize the server metadata structure -
    //  IF THE FILE IS OPENED FIRST TIME
    if((read_bytes=fread((char *)&s_mdata ,sizeof(char),
            sizeof(struct server_info),fp_log)) < sizeof(struct server_info)){
        //s_mdata.num_of_db=0;
        s_mdata.lts=0;
        s_mdata.update_seq=0;
//        s_mdata.num_global_updates=0;

        //  FLUSH THE FIRST SERVER METADATA ON DISC
        if((write_bytes=fwrite((char *)&s_mdata ,sizeof(char),
            sizeof(struct server_info),fp_log)) == sizeof(struct server_info)){
            PDEBUG_REPLICATION("Successfully written\n");
            fflush(fp_log);
        }
        else
            PDEBUG_REPLICATION("fflush testing failed\n");
    }
    else{
       // PDEBUG_REPLICATION("num_of_db=%d\n", s_mdata.num_of_db);
        PDEBUG_REPLICATION("lts=%d\n", s_mdata.lts);
        PDEBUG_REPLICATION("num_local_updates=%d\n", s_mdata.update_seq);


        // Just for testing.. should be removed
/*        struct update_record test_rec;

        if (search_log(5,1, &test_rec)== -1)
            printf("Santosh bahir log is not present\n");
        else{
            printf("DISCOVERY CHANNEL\n");
            print_update(&test_rec);
        }*/
    }
    syncOn=false;

    return;
}


void get_db(const char *db_name, struct db_record *db_rec)
{
    struct msg_update *msg_update=alloc_msg_update();
    struct update_record *rec_update=alloc_update_record();
    char fname[FILE_NAME_SIZE];
    FILE *tmp=NULL;

    create_db_fname(db_name, fname);
/*    if(search_db_name(fp, db_name, (char *)db_rec)){
        return;
    }
*/
    if((tmp=fopen(fname, "r+"))!=NULL){
        fclose(tmp);
    }
    else{
        if((tmp=fopen(fname, "w"))==NULL){
            printf("Failed to create file...exiting\n"); 
            exit(0);
        }

        // Build lts and update_seq
        s_mdata.lts=s_mdata.lts+1;
        s_mdata.update_seq=s_mdata.update_seq+1;

        // Build update Record
        rec_update->lts=s_mdata.lts;
        rec_update->server_id=my_server_id;
        rec_update->update_num=s_mdata.update_seq;
        rec_update->op='a';
        rec_update->entity_type=DB;
        memcpy(rec_update->db_name, db_name, DB_NAME_SIZE);
        rec_update->obj_ver=0;
        memcpy(rec_update->obj, "DISCUSS-BOARD", OBJ_SIZE);
        
        // Build update Message
        msg_update->packet_type=packet_Update;
        msg_update->update=*rec_update;

        // Write to log file
        append(fp_log, sizeof(struct update_record),(char *)rec_update);
        fflush(fp_log);

        /*Send update message*/
        /*Multicasting request to replication servers for creating new db*/
        SP_multicast(
                        Mbox, 
                        AGREED_MESS,
                        rs_group, 
                        1, 
                        sizeof(struct msg_update), 
                        (char *)msg_update
                    );
    }

    /*send data to client*/
    memcpy(db_rec->db_file, fname, FILE_NAME_SIZE);
    memcpy(db_rec->db_name, db_name, DB_NAME_SIZE);
    db_rec->num_of_obj=0;

    return;    
}


void update_replicas(const struct req_client *ptr_cr)
{
    int ret;
   
    struct msg_update *msg_update=alloc_msg_update();   
    struct update_record *rec_update=alloc_update_record();


    s_mdata.lts=s_mdata.lts+1;
    s_mdata.update_seq=s_mdata.update_seq+1;
    // Build the update record
    rec_update->lts=s_mdata.lts;    /*Lamport Timestamp*/
    rec_update->server_id=my_server_id;
    rec_update->update_num=s_mdata.update_seq; //TBD;
    rec_update->op=ptr_cr->op;
    rec_update->entity_type=OBJ; /*DB: create db    OBJ: create object*/

    if(ptr_cr->op=='a'){
        rec_update->obj_id=s_mdata.lts;
        rec_update->server_index=my_server_id;
    }
    else{
        rec_update->obj_id=ptr_cr->obj_id;
        rec_update->server_index=ptr_cr->server_index;
    }

    memcpy(rec_update->db_name, ptr_cr->db_name, DB_NAME_SIZE);
    rec_update->obj_ver=-1;
    memcpy(rec_update->obj, ptr_cr->obj, OBJ_SIZE);
 
    // Build the update message.
    msg_update->packet_type=packet_Update;
    msg_update->update=*rec_update; 

    //TBD: Write LTS metadata on disc
    // Write to log file
//    append(fp_log, sizeof(struct update_record),(char *)rec_update);
//    fflush(fp_log);
//    fflush(fp);

    apply_update(*((struct spread_packet *)msg_update));
    /*
     * Multicast to replicated server.
     */  
    ret = SP_multicast(
                        Mbox, 
                        AGREED_MESS,
                        rs_group, 
                        1,  
                        sizeof(struct msg_update), 
                        (char *)msg_update
                      );  
    if( ret < 0 )
    {
        SP_error( ret );
        Bye();
    }

    return;
}


bool apply_update(struct spread_packet update_msg)
{
    /*
     *  Write to log file
     *  Apply to database
     */
    
    // Write to log file
    struct msg_update *msg_update;
    struct update_record *rec_update;

//    syncOn=false;

    msg_update=(struct msg_update *)(&update_msg);
    rec_update=&(msg_update->update);
    print_update(rec_update);
    /*
     * TBD: if this update is already received then discard it
     * TBD: after messages is received then update the corresponding lts and msg
     * index
     */

    /*
     * if my udpate then do not write to log file as I have done it while
     * sending
     */
    //if(!syncOn && (rec_update->server_id!=my_server_id)){
    if(!syncOn){
        append(fp_log, sizeof(struct update_record),(char *)rec_update);
    }
    
    // update lts
    if(rec_update->lts > s_mdata.lts){
        s_mdata.lts=rec_update->lts;
    }

    if(syncOn){
        if((rec_update->lts > s_mdata.server_lts[(rec_update->server_id)-1]) &&
           (rec_update->update_num > s_mdata.server_updates[(rec_update->server_id)-1]))
        {
            append(fp_log, sizeof(struct update_record),(char *)rec_update);
        }
        else{
            printf("This message is already received. Hence discard it\n");
                return false;
        }
    } 

    s_mdata.server_lts[(rec_update->server_id)-1]     = rec_update->lts;
    s_mdata.server_updates[(rec_update->server_id)-1] = rec_update->update_num;

    write_server_info();
    fflush(fp_log);

    switch(rec_update->op){
        case 'a':
                apply_create(rec_update);
                break;
        case 'e':
                apply_edit(rec_update);
                break;
        case 'd':
                apply_delete(rec_update);
                break;
        default:
                PDEBUG_REPLICATION("unknown operation received\n");
                break;
    }

    /*
     * Send to upper layer - handleclients layer, if entity type is OBJ.
     * So if any client is joined the db to which this object belongs
     */
/*
    if(rec_update->entity_type == OBJ){
        struct resp_client response;
        response.packet_type=packet_Response;
        response.op=rec_update->op;
        response.obj_id=rec_update->obj_id;
        response.server_index=rec_update->server_index;
        memcpy(response.db_name, rec_update->db_name, DB_NAME_SIZE); 
        memcpy(response.obj, rec_update->obj, OBJ_SIZE);

        serve_clients(response);
    }*/
    return true;
}

void send_response(struct update_record *rec_update)
{
    if(rec_update->entity_type == OBJ){
        struct resp_client response;
        response.packet_type=packet_Response;
        response.op=rec_update->op;
        response.obj_id=rec_update->obj_id;
        response.server_index=rec_update->server_index;
        memcpy(response.db_name, rec_update->db_name, DB_NAME_SIZE);
        memcpy(response.obj, rec_update->obj, OBJ_SIZE);

        serve_clients(response);
    }
    return;
}


bool apply_create(struct update_record *rec_update)
{
    char db_file[FILE_NAME_SIZE];
                /* If the DB is created, 
                 *  - Check if the record for this db name is present
                 *  - IF present
                 *     $ ignore the request
                 *  - ELSE
                 *     $ Create the file for db
                 *     $ Create db record and write in db-metadata file   
                 */

                if(rec_update->entity_type == DB){
/*
                    if(search_db_name(fp, rec_update->db_name, (char *)&db_rec_buf)){
                        // Ignore the creation of db as db already exists
                        ;
                    }
                    else{*/
                    {
                        struct db_record *rec_db=alloc_db_record();
                        int fd;

                        PDEBUG_REPLICATION("Received db name:%s\n",rec_update->db_name);
                        // Create the db_record and file for new db 
                        memcpy(rec_db->db_name,rec_update->db_name, \
                        DB_NAME_SIZE);
                        create_db_fname(rec_db->db_name,rec_db->db_file);

                        fd=open(rec_db->db_file, O_RDWR | O_CREAT, S_IRWXU);
                        close(fd);

                        // write the record to db file
/*                        append(fp, sizeof(struct db_record), (char *)rec_db);
                        PDEBUG_REPLICATION("This is db file name%s\n",rec_db->db_file);
                        fflush(fp);*/
                    }
                }
                if(rec_update->entity_type == OBJ){
                    // get the file pointer for DB file and append the record
/*                    if(!search_db_name(fp, rec_update->db_name, (char *)&db_rec_buf)){
                        PDEBUG_REPLICATION("The db record for db_name=%s"
                                            "should have present... exiting\n",rec_update->db_name);
                        exit(0);
                    }
                    else{*/
                    {
                        FILE *fp_db;
                        struct obj_record *rec_obj=alloc_obj_record();

                        // BUild the object to be written in file
                        rec_obj->obj_id         =rec_update->obj_id;
                        rec_obj->server_index   =rec_update->server_index;
                        rec_obj->obj_ver        =rec_update->obj_ver;
                        rec_obj->status         =1;
                        memcpy(rec_obj->db_name, rec_update->db_name, DB_NAME_SIZE);
                        memcpy(rec_obj->object, rec_update->obj, OBJ_SIZE);

                        create_db_fname(rec_obj->db_name, db_file);
                        if((fp_db=fopen(db_file, "r+"))==NULL){
                            printf("%s, %d: The db file should have present... exiting\n", __func__, __LINE__);
                            //exit(0);
                        }
                        else{
                            if((fp_db=fopen(db_file, "a+"))==NULL){
                                printf("%s, %d: Failed to create new file\n", __func__, __LINE__);
                            }
                            
                        }

                        append(fp_db, sizeof(struct obj_record), (char *) rec_obj);
                        printf("written to db file\n");
                        fflush(fp_db);
                        fclose(fp_db);
                    }
                }

    return true;
}

bool apply_edit(struct update_record *rec_update)
{
    char db_file[FILE_NAME_SIZE];
                // get the file pointer for DB file and append the record
                // Search in the file the record and update the record                
/*                if(!search_db_name(fp, rec_update->db_name, (char *)&db_rec_buf)){
                    PDEBUG_REPLICATION("The db record should have present... exiting\n");
                    exit(0);
                }*/
                {
                    FILE *fp_db;
                    struct obj_record *rec_obj=alloc_obj_record();
                    off_t offset;


                    create_db_fname(rec_update->db_name, db_file);
                    if((fp_db=fopen(db_file, "r+")) == NULL){
                        printf("%s, %d: The db file should have present... exiting\n", __func__, __LINE__);
                        exit(0);
                    }

                    offset=search_obj(db_file, rec_update->obj_id,\
                                    rec_update->server_index, rec_obj);

                    if(offset != -1){
                        // Fill the fields from the received update
                        rec_obj->obj_ver=rec_update->obj_ver;
                        rec_obj->status=1;
                        memcpy(rec_obj->db_name, rec_update->db_name, OBJ_SIZE);
                        memcpy(rec_obj->object, rec_update->obj, OBJ_SIZE);
                        write_obj(fp_db, offset, (char *)rec_obj);
                        fflush(fp_db);
                    }
                    else
                        return false;
                }
    return true;
}



bool apply_delete(struct update_record *rec_update)
{
    char db_file[FILE_NAME_SIZE];
                 // get the file pointer for DB file and append the record
                 // Search in the file the record and mark as deleted
 /*
                if(!search_db_name(fp, rec_update->db_name, (char *)&db_rec_buf)){
                    PDEBUG_REPLICATION("The db record should have present... exiting\n");
                    exit(0);
                }
                else{
*/
                {
                    FILE *fp_db;
                    struct obj_record *rec_obj=alloc_obj_record();
                    off_t offset;

                    create_db_fname(rec_update->db_name, db_file);
                    if((fp_db=fopen(db_file, "r+"))==NULL){
                        printf("%s, %d: The db file should have present... exiting\n", __func__, __LINE__);
                        exit(0);
                    }

                    offset=search_obj(db_file, rec_update->obj_id,\
                                  rec_update->server_index, rec_obj);

                    if(offset != -1){
                        rec_obj->status=0;
                        write_obj(fp_db, offset, (char *)rec_obj);
                        fflush(fp_db);
                    }

                    offset=search_obj(db_file,rec_update->obj_id, rec_update->server_index, rec_obj);

                    print_object(rec_obj);
                }  
    
    return true;
}


void create_db_fname(const char *db_name, char *fname)
{
    char extn[]=".db";
    
    sprintf(fname, "%s", db_name);
    strcat(fname,extn);
}

void print_update(struct update_record *rec_update)
{
    printf("\n------------------------------------------------------------------\n");
    printf("\nUPDATE MESSAGE:\n");
    printf("\tlts         : %d\n", rec_update->lts);
    printf("\tserver_id   : %d\n", rec_update->server_id);
    printf("\tmsg_seq     : %d\n", rec_update->update_num);
    printf("\top          : %c\n", rec_update->op);
    printf("\tentity_type : %c\n", rec_update->entity_type);
    printf("\tobj_id      : %d\n", rec_update->obj_id);
    printf("\tserver_index: %d\n", rec_update->server_index);
    printf("\tobj_ver     : %d\n", rec_update->obj_ver);
    printf("\tdb_name     : %s\n", rec_update->db_name);
    printf("\tobj         : %s\n", rec_update->obj);
    printf("------------------------------------------------------------------\n");

    return;
}


void print_object(struct obj_record *obj)
{
    printf("\n------------------------------------------------------------------\n");
    printf("\nOBJECT RECORD:\n");
    printf("\tobj_id         : %d\n", obj->obj_id);
    printf("\tserver_index   : %d\n", obj->server_index);
    printf("\tobj_ver        : %d\n", obj->obj_ver);
    printf("\tstatus         : %d\n", obj->status);
    printf("\tdb_name        : %s\n", obj->db_name);
    printf("\tobject         : %s\n", obj->object);
    printf("------------------------------------------------------------------\n");

    return;
}

void print_server_info()
{
    int i;
    printf("\n------------------------------------------------------------------\n");
    printf("\nMY DATA:\n");
    printf("\tlts           : %d\n", s_mdata.lts);
    printf("\tupdate_seq    : %d\n", s_mdata.update_seq);

    printf("\tserver_lts     = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d]\n", s_mdata.server_lts[i-1]);
        else
            printf("%d\t", s_mdata.server_lts[i-1]);
    }

    printf("\tserver_update  = [");
    for(i=1; i<=NUM_OF_SERVERS; i++){
        if(i==NUM_OF_SERVERS)
            printf("%d]\n", s_mdata.server_updates[i-1]);
        else
            printf("%d\t", s_mdata.server_updates[i-1]);
    }

    printf("\n\n------------------------------------------------------------------\n");

    return;
}

