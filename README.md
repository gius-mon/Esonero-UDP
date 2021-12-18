
       **Esonero UDP "Calculator"**
	

Author: Monitillo Giuseppe (Matricola 716699)
	      Vicenti Rosa (Matricola 701559)


Client-side: The client must decide whether to set
	           the host name and port or use the 
	           default ones (127.0.0.1:50000)

	           Must insert a single string that represents
             the operation to be performed in the form 
             "operator operand operand", ex. ("+ 5 6")

	           Will receive a string representing the result
             from the server will have to decide whether
             to carry out other operations or to terminate
             the program by sending the "=" character


Server-side: The server reads what is sent by the client,
             performs the requested operation and sends
             the result to the client
    
             The server never ends its process and must
	           be able to accept a queue of up to 5 clients
