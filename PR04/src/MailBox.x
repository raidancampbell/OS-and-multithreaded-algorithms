 typedef string str_t<81>;/*one more byte for dat null terminator*/

 struct user {
    string_wrapper hostname;
    int uuid;
 };

 struct string_wrapper {
     str_t data;
 };

 struct retrieve_message_params {
    user given_user;
    int message_number;
    string_wrapper message;
 };

 struct delete_message_params {
    user given_user;
    int message_number;
 };

 program DISPLAY_PRG {
        version DISPLAY_VER {

          void start(user) = 1;                                             //C
          void quit(user) = 2;                                              //
          string_wrapper retrieve_message (retrieve_message_params) = 3;    //
          /*I have no idea what the string parameter was for there*/
          string_wrapper list_all_messages (user) = 4;                      //
          void delete_message(delete_message_params) = 5;                   //

        } = 1;
 } = 0x20000001;