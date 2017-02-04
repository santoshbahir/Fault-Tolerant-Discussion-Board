Pseudocode:

Main()
{
    Call  Connect_to_spread_daemon()
***    Call Handle_ctrl_quit()  ****
    Call Display_menu()


    While(TRUE)
    {
        ***Handle_deamon_crash() // to check continuously if the daemon is alive or not
        On user input: call handle_user_input()
    }
}

Connect_to_spread_daemon()
{
 Call SP_connect_timeout()
}

Handle_ctrl_quit()
{
Call signal(SIGINT,ctrl_quit) 
}

Ctrl_quit()
{
Display: user pressed ctrl+c
exit
}

Handle_daemon_crash()
{
 Display “ Spread Daemon is dead”
Exit ? or should we wait until daemon is restarted?
}


Display_menu()
{
Display: 
1.Connect to Server
2.Disconnect from server // or just display press ctrl+c to disconnect from server instead of a separate menu option????
3.Join a discussion
4.Append object
5.Edit object
6.Delete object
}
Handle_user_input()
{
Case “connect_to_server”:
	Call connect_to_server()
Break

Case “disconnect_from_server”:
	Call disconnect_from_server()
Break

Case “join discussion”:
	Call is_connected_to_server()
	If YES: call join_discussion()
	If NO: display “User not connected to server”
Break

Case “Append Object”:
	Call is_connected_to_server()
		If YES: call is_connected_to_dboard()
			If YES: call display_board()
			If NO: display “User havnt joined a board yet”
		If NO: display “ user not connected to the server”
Break

Case “Edit Object”:
	Call is_connected_to_server()
		If YES: call is_connected_to_dboard()
			If YES: call edit_object()
			If NO: display “User havnt joined a board yet”
		If NO: display “ user not connected to the server”
Break



Case “Append Object”:
	Call is_connected_to_server()
		If YES: call is_connected_to_dboard()
			If YES: call delete_object()
			If NO: display “User havnt joined a board yet”
		If NO: display “ user not connected to the server”
Break

Case “View of the Servers”:
Display the view of the servers in the current server’s network component
Break 
Default  “view of the Servers”
	
??? what is the view of the servers ----????
 Is it the current status of the discussion board according to each server?



