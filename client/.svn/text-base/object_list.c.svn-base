#include "object_list.h"
#include "types.h"
#include "spread_comm.h"

struct db_object *head;
void init_list()
{
    head=NULL;	
}

bool is_empty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void re_init_list()
{
/*    struct db_object *cur;
    struct db_object *tmp;

    tmp=head;
    cur=tmp;

    if(!is_empty()){
        do{
            cur=tmp;
            tmp=tmp->next;
            
            del(cur);
        }while(tmp!=NULL);
    }

    head=NULL;
*/

    while(!is_empty()){
        del(head);
    }
    return;
}

// mkg_s

void insert(struct db_object *obj)
{
    struct db_object *curr; 

    struct db_object *old_obj;

    bool isLast=true;

    
//     printf("INSERTTTTTTTTTT - test\n");
    // object is present so replace it
    if((old_obj=search(obj->obj_id,obj->server_index))!=NULL){
//        printf("INSIDE IF\n");
//        printf("Object is present..\n");
        memcpy(old_obj->object,obj->object,OBJ_SIZE);
    }
    // object is not present so insert object at proper place
    else{
  
//        printf("INSIDE ELSE\n");
        if(is_empty()){
            obj->next=obj;
            obj->prev=obj;
            head=obj;
        }
        
        else{
            curr=head;
            isLast=true;

            do{
//                printf("INSIDE LIST : %d\t%d\n",curr->obj_id, curr->server_index);
//                printf("RECVD OBJ   : %d\t%d\n", obj->obj_id, obj->server_index);
                // if prev <  obj < curr; keep going ahead
                if( curr->obj_id < obj->obj_id ){
                        ;
                }
                else{
                    if(curr->obj_id == obj->obj_id ){
                        if(curr->server_index <= obj->server_index){
                            ;
                        }
                        else{
                            isLast=false;
                            break;
                        }
                    }
                    else{
                        isLast=false;
                        break;
                    }
                }
                curr=curr->next;    
            }while(curr!=head);

            if(!isLast && curr == head)
                head = obj;
            // add before current
            obj->next=curr;
            obj->prev=curr->prev;

            curr->prev->next=obj;
            curr->prev=obj;
        } 


    }
    return;                    
}

// mkg_e

void append(struct db_object *obj)
{

    if(is_empty()){
        obj->next=obj;
        obj->prev=obj;
        head=obj;
    }
    else{
        obj->next=head;
        obj->prev=head->prev;
        head->prev->next=obj;
        head->prev=obj;
    }
    return;
}

void edit(struct db_object *obj)
{

    struct db_object *old_obj;
    if((old_obj=search(obj->obj_id,obj->server_index))==NULL){
//        printf("Object is not present... exiting\n");
        return;
    }

    memcpy(old_obj->object,obj->object,OBJ_SIZE);

}

void del(struct db_object *obj)
{
    if(head==obj && head->next==obj){
        head=NULL;
        return;
    }
    else{
        if(head==obj){
            head=obj->next;
        }

        obj->prev->next=obj->next;
        obj->next->prev=obj->prev;
    }
    return;
}

struct db_object *search(int lts,int server_index)
{

	struct db_object *current = head;
    static int i=0;

    if(!is_empty()){
        do{	
	    	if (lts == current->obj_id && \
                server_index == current->server_index)
    		{
                return current;					
            }
	    	current = current->next;			
        }while(current!=head);
    }

    return NULL;
}


void append_obj(char *obj)
{
    int ret;
    /*
     * Build the packet and send to server
     */ 

    struct req_client client_request;

    client_request.packet_type  = packet_Client;
    client_request.op           = 'a';
    client_request.obj_id       = -997;
    client_request.server_index = -997;
    memcpy(client_request.db_name,dboard,DB_NAME_SIZE);
    memcpy(client_request.obj,obj,OBJ_SIZE);
    
    ret= SP_multicast(  
                         Mbox, 
                         FIFO_MESS,
                         server_list[cs_index-1],
                         1,
                         sizeof(struct req_client),
                         (char*)&client_request 
                     );

    if(ret<0) 
    {
        SP_error( ret );
        Bye();
    }
    return;
}

struct db_object *search_obj(int obj_num)
{
    int i=0;
    struct db_object *current = head;

    while(current && (i<obj_num)){
        current = current->next;    
        i++;
    }   
    return current;    
}

void edit_obj(int obj_num, char *obj)
{
    struct db_object *ob;
    int ret;
   
    if((ob=search_obj(obj_num))==NULL){
//        printf("Object is not present... exiting\n");
//        exit(0);
          return;
    }
    /*
     * Build the packet and send to server
     */

    struct req_client client_request;

    client_request.packet_type  = packet_Client;
    client_request.op           = 'e';
    client_request.obj_id       = ob->obj_id;
    client_request.server_index = ob->server_index;
    memcpy(client_request.db_name,dboard,DB_NAME_SIZE);
    memcpy(client_request.obj,obj,OBJ_SIZE);

    ret= SP_multicast(
                         Mbox,
                         FIFO_MESS,
                         server_list[cs_index-1],
                         1,
                         sizeof(struct req_client),
                         (char*)&client_request
                     );

    if(ret<0)
    {
        SP_error( ret );
        Bye();
    }
    return;
}


void delete_obj(int obj_num)
{
    struct db_object *ob;
    int ret;

    if((ob=search_obj(obj_num))==NULL){          
//        printf("Object is not present... exiting\n");
//        exit(0);
            return;        
    }
    /*
     * Build the packet and send to server
     */

    struct req_client client_request;

    client_request.packet_type  = packet_Client;
    client_request.op           = 'd';
    client_request.obj_id       = ob->obj_id;
    client_request.server_index = ob->server_index;
    memcpy(client_request.db_name,dboard,DB_NAME_SIZE);
    memcpy(client_request.obj,ob->object,OBJ_SIZE);

    ret= SP_multicast(
                         Mbox,
                         FIFO_MESS,
                         server_list[cs_index-1],
                         1,
                         sizeof(struct req_client),
                         (char*)&client_request
                     );

    if(ret<0)
    {
        SP_error( ret );
  //      Bye();
    }
    return;
}

//display with linenumbers and the users would select the linenumber to delete the object.
void display_board()
{
	struct db_object *current;
	int line_no=1;
	
    if(head != NULL){
	current = head;

    if(strcmp(curr_db, "-1") == 0)
    	printf("\nDiscussion Board: \n");
    else
    	printf("\nDiscussion Board: %s\n", curr_db);

	printf("-----------------\n");

	printf("\n");

	printf("\tLineno\t\tObject\n");
	printf("-------------------------------------------------------------------------\n");
        do
	   {    
		printf("\t%d\t\t%s\n",line_no++,current->object);
		current = current->next;
	   }while(current != head);
    }

    obj_num=line_no-1; 
	
}

