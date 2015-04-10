/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _MAILBOX_H_RPCGEN
#define _MAILBOX_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


typedef char *str_t;
#ifdef __cplusplus
extern "C" bool_t xdr_str_t(XDR *, str_t*);
#elif __STDC__
extern  bool_t xdr_str_t(XDR *, str_t*);
#else /* Old Style C */
bool_t xdr_str_t();
#endif /* Old Style C */


struct string_wrapper {
	str_t data;
};
typedef struct string_wrapper string_wrapper;
#ifdef __cplusplus
extern "C" bool_t xdr_string_wrapper(XDR *, string_wrapper*);
#elif __STDC__
extern  bool_t xdr_string_wrapper(XDR *, string_wrapper*);
#else /* Old Style C */
bool_t xdr_string_wrapper();
#endif /* Old Style C */


struct retrieve_message_params {
	user given_user;
	int message_number;
	string_wrapper message;
};
typedef struct retrieve_message_params retrieve_message_params;
#ifdef __cplusplus
extern "C" bool_t xdr_retrieve_message_params(XDR *, retrieve_message_params*);
#elif __STDC__
extern  bool_t xdr_retrieve_message_params(XDR *, retrieve_message_params*);
#else /* Old Style C */
bool_t xdr_retrieve_message_params();
#endif /* Old Style C */


struct delete_message_params {
	user given_user;
	int message_number;
};
typedef struct delete_message_params delete_message_params;
#ifdef __cplusplus
extern "C" bool_t xdr_delete_message_params(XDR *, delete_message_params*);
#elif __STDC__
extern  bool_t xdr_delete_message_params(XDR *, delete_message_params*);
#else /* Old Style C */
bool_t xdr_delete_message_params();
#endif /* Old Style C */


#define DISPLAY_PRG ((rpc_uint)0x20000001)
#define DISPLAY_VER ((rpc_uint)1)

#ifdef __cplusplus
#define start ((rpc_uint)1)
extern "C" void * start_1(user *, CLIENT *);
extern "C" void * start_1_svc(user *, struct svc_req *);
#define quit ((rpc_uint)2)
extern "C" void * quit_1(user *, CLIENT *);
extern "C" void * quit_1_svc(user *, struct svc_req *);
#define retrieve_message ((rpc_uint)3)
extern "C" string_wrapper * retrieve_message_1(retrieve_message_params *, CLIENT *);
extern "C" string_wrapper * retrieve_message_1_svc(retrieve_message_params *, struct svc_req *);
#define list_all_messages ((rpc_uint)4)
extern "C" string_wrapper * list_all_messages_1(user *, CLIENT *);
extern "C" string_wrapper * list_all_messages_1_svc(user *, struct svc_req *);
#define delete_message ((rpc_uint)5)
extern "C" void * delete_message_1(delete_message_params *, CLIENT *);
extern "C" void * delete_message_1_svc(delete_message_params *, struct svc_req *);

#elif __STDC__
#define start ((rpc_uint)1)
extern  void * start_1(user *, CLIENT *);
extern  void * start_1_svc(user *, struct svc_req *);
#define quit ((rpc_uint)2)
extern  void * quit_1(user *, CLIENT *);
extern  void * quit_1_svc(user *, struct svc_req *);
#define retrieve_message ((rpc_uint)3)
extern  string_wrapper * retrieve_message_1(retrieve_message_params *, CLIENT *);
extern  string_wrapper * retrieve_message_1_svc(retrieve_message_params *, struct svc_req *);
#define list_all_messages ((rpc_uint)4)
extern  string_wrapper * list_all_messages_1(user *, CLIENT *);
extern  string_wrapper * list_all_messages_1_svc(user *, struct svc_req *);
#define delete_message ((rpc_uint)5)
extern  void * delete_message_1(delete_message_params *, CLIENT *);
extern  void * delete_message_1_svc(delete_message_params *, struct svc_req *);

#else /* Old Style C */
#define start ((rpc_uint)1)
extern  void * start_1();
extern  void * start_1_svc();
#define quit ((rpc_uint)2)
extern  void * quit_1();
extern  void * quit_1_svc();
#define retrieve_message ((rpc_uint)3)
extern  string_wrapper * retrieve_message_1();
extern  string_wrapper * retrieve_message_1_svc();
#define list_all_messages ((rpc_uint)4)
extern  string_wrapper * list_all_messages_1();
extern  string_wrapper * list_all_messages_1_svc();
#define delete_message ((rpc_uint)5)
extern  void * delete_message_1();
extern  void * delete_message_1_svc();
#endif /* Old Style C */

#endif /* !_MAILBOX_H_RPCGEN */
