#include "spread_comm.h"
#include "types.h"
#include "object_list.h"
#include "server_comm.h"


void print_response(struct resp_client *resp);	

void create_client_server_group(int server_index)
{
	int process_id;
	char pid[50];
	
	process_id = getpid();//Set the process id of each client appended with '_client' as a unique group name	
	sprintf(pid, "%d",process_id);
	
	sprintf(client_server_group,"%s","client_");
	strcat(client_server_group,pid);
	strcat(client_server_group,"_");
	strcat(client_server_group,server_list[server_index-1]);


	return; 

}

void join_group(const char *group_name){

	int ret;
	ret = SP_join(Mbox, group_name);

	if( ret < 0 )
		SP_error(ret);
	else
	printf("Client joined %s group\n",group_name);

	return; 
}

void connect_spread(){

	int ret;
	sp_time test_timeout;
	test_timeout.sec = 5;
	test_timeout.usec = 0;
	sprintf(Spread_name,"22222@localhost");
	ret = SP_connect_timeout( Spread_name, User, 0, 1, &Mbox, Private_group, test_timeout );
	printf("SPREAD NAME = %s\n",Spread_name);
	
	if( ret != ACCEPT_SESSION )
	{
		SP_error(ret);
		Bye();
	}

	printf("User: connected to %s with private group %s\n", Spread_name, Private_group );	

}

void handle_ctrl_quit()
{

	(void) signal(SIGINT,ctrl_quit);
	return;

}

void ctrl_quit(int sig)
{
	printf("\nCtrl+c signal (Signal ID = %d) is received So Quitting the program!!\n", sig);
	exit(sig);
}  

void Bye()
{
    To_exit = 1;

    printf("\nBye.\n");

    exit(0);
}

/*
		if (!strcmp(sender,client_server_group))
		{
			client_connected = 1;
		}
*/


void Read_message()
{

    static	char        mess[MAX_MESSLEN];
	char		        sender[MAX_GROUP_NAME];
	char		        target_groups[MAX_MEMBERS][MAX_GROUP_NAME];
    membership_info     memb_info;
    vs_set_info         vssets[MAX_VSSETS];
    unsigned int        my_vsset_index;
    int                 num_vs_sets;
    char                members[MAX_MEMBERS][MAX_GROUP_NAME];
	int		            num_groups;
	int		            service_type;
	int16		        mess_type;
	int		            endian_mismatch;
	int		            i,j;
	int		            ret;
	int                 dis;

   
	struct resp_client *resp=(struct resp_client *)mess; // This is the response from the server


    sp_time test_timeout;
    test_timeout.sec = 5;
    test_timeout.usec = 0;

    service_type = 0;

    ret = SP_receive( Mbox, &service_type, sender, 100, &num_groups, target_groups,
          &mess_type, &endian_mismatch, sizeof(mess), mess );
       
    
    if(ret == CONNECTION_CLOSED)
    {
        printf(" Daemon is crashed : Waiting for the Daemon to restart\n");
        /* Wait until the daemon is started and connect to it */            
        dis = SP_disconnect( Mbox );    

        if(dis == 0 )
            printf("Successfully disconnected\n");
        else
            printf("Could not disconnect previous Mbox\n");
                     
        for(;;)
        {
            ret = SP_connect_timeout( Spread_name, User, 0, 1, &Mbox, Private_group, test_timeout );
     
            if( ret == ACCEPT_SESSION )
            {
                
                printf("User: Reconnected to %s with private group %s\n", Spread_name, Private_group );
                break;                                
            }
        }               
    }

	//printf("\n============================\n");
	if( ret < 0 ) 
	{
        if ( (ret == GROUPS_TOO_SHORT) || (ret == BUFFER_TOO_SHORT) ) {
            service_type = DROP_RECV;
            printf("\n========Buffers or Groups too Short=======\n");
            ret = SP_receive( Mbox, &service_type, sender, MAX_MEMBERS, &num_groups, target_groups, 
                             &mess_type, &endian_mismatch, sizeof(mess), mess );
        }
    }
		
    if(ret < 0)
    {
		if(!To_exit)
		{
			SP_error(ret);
			printf("\n============================\n");
			printf("\nBye.\n");
		}
		exit(0);
	}

	if(Is_regular_mess(service_type))
	{
                    system("clear"); //SSB

        struct db_object *obj,*old_obj;;

        if((obj=(struct db_object *)malloc(sizeof(struct db_object)))==NULL){
            printf("\nMalloc failed\n");
            exit(0);
        }
		
        
//	    print_response(resp);	
		if(resp->packet_type == packet_Response )
		{
		}	
			/* Use if OP is being sent in the resp */
			
			switch(resp->op)
			{
				case 'j':
					//joined_db = 1;
					//insert(resp->lts,resp->server_index,resp->obj);
					//insert(resp->lts,resp->server_index,resp->obj);
					printf("Connected to Discussion Board \"%s\"\n", resp->db_name);
					break;

				case 'a':
                    obj->obj_id=resp->obj_id;
                    obj->server_index=resp->server_index;
		      		memcpy(obj->object, resp->obj, OBJ_SIZE);
		       		insert(obj);
					
					break;
				case 'd':
					
				    if((old_obj = search(resp->obj_id,resp->server_index))!=NULL){
    					del(old_obj);
                    }
					//insert(old_obj);
					
					break;

				case 'e':
				
					obj->obj_id = resp->obj_id;
					obj->server_index = resp->server_index;
					memcpy(obj->object,resp->obj,OBJ_SIZE);
					edit(obj);
					
					break;

           		case 'v':
           			viewRequested=true;
           			break;
					
           		default:
           			printf("Unsupported operation...exiting\n");
           			exit(0);
			}
			
				
		mess[ret] = 0;

		
	}
    else if(Is_membership_mess(service_type))
    {
        
        system("clear"); //SSB
        ret = SP_get_memb_info( mess, service_type, &memb_info );

        if(ret<0){
            printf("BUG: membership message does not have valid body\n");
            SP_error( ret );
            exit( 1 );
        }
        if(Is_reg_memb_mess(service_type))
		{
			
/*			printf("Received REGULAR membership for group %s with %d members, where I am member %d:\n",
				sender, num_groups, mess_type );

			for( i=0; i < num_groups; i++ )
				printf("\t%s\n", &target_groups[i][0] );

			printf("grp id is %d %d %d\n",memb_info.gid.id[0], memb_info.gid.id[1], memb_info.gid.id[2] );
*/			

			if(strcmp(sender,client_server_group)==0 && num_groups == 2)
			{
				printf("Client Connected to Server %d\n",cs_index);
				client_connected = 1; 

                if(is_empty()){
            //        printf("This indeed should be empty\n");
                }
			}
			else
			{
				printf("Please Retry, server is not available \n");
				client_connected = 0; 
			}
			
			if( Is_caused_join_mess( service_type ) )
			{
//				printf("Due to the JOIN of %s\n", memb_info.changed_member );
			}else if( Is_caused_leave_mess( service_type ) ){
//				printf("Due to the LEAVE of %s\n", memb_info.changed_member );
                
        
                // Server left, so reconnect to server
                if(strcmp(client_server_group, sender) == 0 &&
                    strcmp(Private_group, memb_info.changed_member) != 0){
                    disconnect=true;
                    SP_leave(Mbox, client_server_group);
                    re_init();
                }

        
			}else if( Is_caused_disconnect_mess( service_type ) ){
//				printf("Due to the DISCONNECT of %s\n", memb_info.changed_member );


                // Server left, so reconnect to server
                if(strcmp(client_server_group, sender) == 0 &&
                    strcmp(Private_group, memb_info.changed_member) != 0){
                    disconnect=true;
                    SP_leave(Mbox, client_server_group);
                    re_init();
                }



			}else if( Is_caused_network_mess( service_type ) ){
//				printf("Due to NETWORK change with %u VS sets\n", memb_info.num_vs_sets);
                num_vs_sets = SP_get_vs_sets_info( mess, &vssets[0], MAX_VSSETS, &my_vsset_index );

                if (num_vs_sets < 0) {
  //                  printf("BUG: membership message has more then %d vs sets. Recompile with larger MAX_VSSETS\n", MAX_VSSETS);
                    SP_error( num_vs_sets );
                    exit( 1 );
                }

                for( i = 0; i < num_vs_sets; i++ )
                {
    /*                printf("%s VS set %d has %u members:\n",
                    (i  == my_vsset_index) ?
                    ("LOCAL") : ("OTHER"), i, vssets[i].num_members ); */
                    ret = SP_get_vs_set_members(mess, &vssets[i], members, MAX_MEMBERS);

                    if (ret < 0) {
//                        printf("VS Set has more then %d members. Recompile with larger MAX_MEMBERS\n", MAX_MEMBERS);
                        SP_error( ret );
                        exit( 1 );
                    }
                    for( j = 0; j < vssets[i].num_members; j++ )
; //                      printf("\t%s\n", members[j] );
               }
		  }

         
		}else if( Is_transition_mess(   service_type ) ) {
//			printf("received TRANSITIONAL membership for group %s\n", sender );
		}else if( Is_caused_leave_mess( service_type ) ){
//			printf("received membership message that left group %s\n", sender );
		}else;// printf("received incorrecty membership message of type 0x%x\n", service_type );
        } 
  
	else if ( Is_reject_mess( service_type ) )
        {
/*		printf("REJECTED message from %s, of servicetype 0x%x messtype %d, (endian %d) to %d groups \n(%d bytes): %s\n",
			sender, service_type, mess_type, endian_mismatch, num_groups, ret, mess );*/
	}else;// printf("received message of unknown message type 0x%x with ret %d\n", service_type, ret);


	display_menu();// Refresh menu
    
    if(viewRequested){
        print_view(resp);
        viewRequested=false;
    }
	fflush(stdout);
}


void print_response(struct resp_client *resp)
{
    printf("Server has sent ...\n");
    printf("packet type         =%d\n",resp->packet_type);
    printf("operation           =%c\n",resp->op);
    printf("obj_id              =%d\n",resp->obj_id);
    printf("server_index        =%d\n",resp->server_index);
    printf("d-board name        =%s\n",resp->db_name);
    printf("object              =%s\n",resp->obj);

    return;
}
