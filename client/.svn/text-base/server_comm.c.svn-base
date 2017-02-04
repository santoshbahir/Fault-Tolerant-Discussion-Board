#include "server_comm.h"

void display_menu()
{
/*
	printf("\n*************************************************************************");
	printf("\n*************************************************************************");
	printf("\n");
*/
//	system("clear");
	/* Display Menu */
	printf("\n");
	printf("User Menu:\n");
	printf("----------\n");
	printf("\n");
	printf("\t1) #Usage: c < server_index > Connect to Server  \n");
	printf("\t2) #Usage: j < board_name >   Join a Discussion  \n");
	printf("\t3) #Usage: a < object >       Append object     \n");
	printf("\t4) #Usage: e < line_number >  Edit Object       \n");
	printf("\t5) #Usage: d < line_number >  Delete Object     \n");
	printf("\t6) #Usage: v                  Print view        \n");
	printf("\n");
	printf("\tq -- quit\n");

    if(disconnect)
    {
        disconnect=false;
        printf("\t Server disconnected, please connect again\n\n");
	}
	/* Display the Discussion board */
	display_board();
	if(viewRequested){
       		;
        	
    	}
    else{
/*	printf("\n*************************************************************************");
	printf("\n*************************************************************************");
	printf("\n");
	printf("\nUser> ");
	fflush(stdout); */ ;
    display_prompt();    
    }
	
	return;
}

void display_prompt()
{
	printf("\n");
	printf("\nUser> ");
	fflush(stdout);
    
    return;
}

void handle_user_input()
{
	
//	PRINTNL;   
//	PINOUT_CLIENT("Entered\n");

	int	    ret;
	int	    i;
	char	command[130]; // SSB: why size 130?
//	char	dboard[DB_NAME_SIZE];
	char	dobject[OBJ_SIZE];
	int 	linenumber;
	

	for(i=0; i<sizeof(command); i++) command[i]=0;
	if(fgets(command, 130, stdin )==NULL) 
        Bye();

	struct req_client client_request;
    int ip_num; // temporary input

	switch( command[0])
	{
        case 'c':
  			/* Based on the server_index the group name will change */
    		ret = sscanf( &command[2], "%d", &ip_num);
	    	if( ret < 1  ) 
		    {
			    printf(" invalid operation \n");
   				break;
    		}

            if( ip_num < 1 || ip_num > 5)
            {
                printf(" Server does not exist, please retry ..\n");
                break;
            }
            
			if(client_connected == 1){
                if(cs_index == ip_num){
                    printf("You are already Connected to %d \n",cs_index);
                    break;
                }

				printf("Already connected to %d.",cs_index);
				printf("disconnecting from %d ",cs_index);
                SP_leave(Mbox, client_server_group);

				printf("Connecting to %d ",ip_num);
                re_init();
			}

            cs_index=ip_num;

	    	printf("Connecting to Server %d\n",cs_index);
		    create_client_server_group(cs_index);
   			join_group(client_server_group);
    		// First the client joins the group ( client,server) and then sends the request to 				
	    	// the server
			
   			client_request.packet_type 	= packet_Client;
    		client_request.op 		= 'c';
	    	client_request.obj_id 		= -999;
		    client_request.server_index	= -999;
   			memcpy(client_request.db_name,"XXX",DB_NAME_SIZE);
    		memcpy(client_request.obj,client_server_group,OBJ_SIZE);
    		//printf("sent group request is %s\n",client_request.obj);
    		printf("Connecting to Server %s\n",server_list[cs_index-1]);
	    		
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

			break;

		case 'j':
            system("clear"); //SSB
			if(client_connected == 0){
				printf("Please Connect to a server \n");
			}
			else{
			    /*
    			if(joined_db == 1){
        			printf("Already connected to a Discussion Board \n");
			    }
			    else{
			    */
			    /* Discussion Board : Initialization of object list*/
    			re_init_list();				
	    		ret = sscanf(&command[2],"%s", dboard);
		    	if(ret<1) 
			    {
				    printf(" invalid operation \n");
    				break;
	    		}
			
                if(strcmp(dboard, curr_db)==0){
				    printf("You are already joined this db \n");
                    break;
                }

			    client_request.packet_type 	= packet_Client;
			    client_request.op 		    = 'j';
			    client_request.obj_id 		= -998;
			    client_request.server_index	= -998;
			    memcpy(client_request.db_name, dboard, DB_NAME_SIZE);

			    // curr_db=-1 : client joining the db for the first time.
			    memcpy(client_request.obj, curr_db, OBJ_SIZE);
			   
			    // If user joins the new dboard, this is switch command and old
			    // dboard name will be sent to server in obj objecct 
			    memcpy(curr_db, dboard, OBJ_SIZE);
                
			    ret=SP_multicast( 
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
			    printf("joined Discussion Board :-> \t%s\n",client_request.db_name);

			    memcpy(curr_db, dboard, DB_NAME_SIZE);
			}
			display_menu();
			break;

		case 'a':
			if(client_connected == 0){ // No connection to server
				printf("Please Connect to a server \n");
			}
			else if( !strcmp(curr_db, "-1")){ // dboard is not joined
				printf("You have not joined any dboard \n");
            }
            else{
    			memcpy(&dobject,&command[2],OBJ_SIZE);
                append_obj(dobject);
			}
			break;

		case 'd':
			
			if(client_connected == 0){ // No connection to server
				printf("Please Connect to a server \n");
			}
			else if( !strcmp(curr_db, "-1")){ // dbaord is not joined
				printf("You have not joined any dboard \n");
            }
			else{		
			    ret = sscanf( &command[2], "%d", &linenumber);
			    if(ret<1) 
			    {
				    printf(" invalid operation \n");
				    break;
			    }
                if(linenumber <= 0 || linenumber > obj_num)
                {
				    printf(" Entered wrong line number \n");
				    break;
                }
               		 delete_obj(linenumber-1);
			}

			break;

		case 'e':
			
			if(client_connected == 0){ // No connection to server
				printf("Please Connect to a server \n");
			}
			else if( !strcmp(curr_db, "-1")){ // dbaord is not joined
				printf("You have not joined any dboard \n");
            }
			else{
    			ret = sscanf( &command[2], "%d", &linenumber);
			    memcpy(dobject,&command[4],OBJ_SIZE);
			    if( ret < 1  ) 
			    {
				    printf(" invalid operation \n");
				    break;
			    }
            
                if(linenumber <= 0 || linenumber > obj_num)
                {
				    printf(" Entered wrong line number \n");
				    break;
                }

			    printf("Editing linenumber %d with object = %s\n",linenumber,dobject);

                edit_obj(linenumber-1, dobject);
			}
			break;

		case 'v':
            if(client_connected == 0){
				printf("Please Connect to a server \n");
                break;
            }
			printf("The Current View of the servers \n");
           	get_view();
			break;

		case 'q':
            SP_leave(Mbox, client_server_group);
			Bye();			    
			break;

		default:
			printf("\nUnknown command\n");
			display_menu();

			break;
	}

//	PINOUT_CLIENT("Entered\n");
//	PRINTNL; 

}

void print_view(struct resp_client *resp)
{
    int i;
    int view[NUM_OF_SERVERS];

    memcpy((char *)view, resp->obj, 5*sizeof(int));

    printf("\n");
    printf("Available Servers:\n");
    printf("------------------\n");
    for(i=0; i<NUM_OF_SERVERS; i++)
    {
        if(view[i] != -1){
            printf("| server-%d | ", view[i]);
        }
    }
    printf("\n");
    printf("\nUser> ");
    fflush(stdout);


    return;
}


void get_view()
{
    int ret;
    struct req_client client_request;

    client_request.packet_type  = packet_Client;
    client_request.op           = 'v';
    client_request.obj_id       = -999;
    client_request.server_index = -999;
    memcpy(client_request.db_name,"No db name",DB_NAME_SIZE);
    memcpy(client_request.obj,"Objec is not required",OBJ_SIZE);
    //printf("sent group request is %s\n",client_request.obj);
    printf("Requested view\n");

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
//    printf("sent connect request\n" );

    return;
}


void re_init()
{
    memcpy(client_server_group, "-1", MAX_GROUP_NAME+8);
    client_connected=0;
    
    obj_num=0;

    memcpy(dboard,"-1", DB_NAME_SIZE);    
    memcpy(curr_db,"-1", DB_NAME_SIZE);    

    re_init_list();

    return;
}
