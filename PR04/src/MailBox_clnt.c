/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "MailBox.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

void *
start_1(argp, clnt)
	user *argp;
	CLIENT *clnt;
{
	static char clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, start, xdr_user, argp, xdr_void, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return ((void *)&clnt_res);
}

void *
quit_1(argp, clnt)
	user *argp;
	CLIENT *clnt;
{
	static char clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, quit, xdr_user, argp, xdr_void, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return ((void *)&clnt_res);
}

char **
retrieve_message_1(argp, clnt)
	retrieve_message_params *argp;
	CLIENT *clnt;
{
	static char *clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, retrieve_message, xdr_retrieve_message_params, argp, xdr_wrapstring, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

message_block *
list_all_messages_1(argp, clnt)
	user *argp;
	CLIENT *clnt;
{
	static message_block clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, list_all_messages, xdr_user, argp, xdr_message_block, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

void *
delete_message_1(argp, clnt)
	delete_message_params *argp;
	CLIENT *clnt;
{
	static char clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, delete_message, xdr_delete_message_params, argp, xdr_void, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return ((void *)&clnt_res);
}

void *
insert_message_1(argp, clnt)
	insert_message_params *argp;
	CLIENT *clnt;
{
	static char clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, insert_message, xdr_insert_message_params, argp, xdr_void, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return ((void *)&clnt_res);
}
