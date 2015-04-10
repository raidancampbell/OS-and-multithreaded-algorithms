 typedef string str_t<81>;/*one more byte for dat null terminator*/
 typedef message_block_type string_wrapper<20>;

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

 struct message_block {
    message_block_type data;
 };

 program DISPLAY_PRG {
        version DISPLAY_VER {

          void start(user) = 1;
          void quit(user) = 2;
          string_wrapper retrieve_message (retrieve_message_params) = 3;
          /*I have no idea what the string parameter was for there*/
          message_block list_all_messages (user) = 4;
          void delete_message(delete_message_params) = 5;

        } = 1;
 } = 0x20000001;