 struct user {
    string hostname<50>;
    int uuid;
 };

 struct retrieve_message_params {
    user given_user;
    int message_number;
 };

 struct delete_message_params {
    user given_user;
    int message_number;
 };

 struct message_block {
    string data<1600>;
 };

 struct insert_message_params {
    user given_user;
    string message<80>;
 };

 program DISPLAY_PRG {
        version DISPLAY_VER {

          void start(user) = 1;
          void quit(user) = 2;
          string retrieve_message (retrieve_message_params) = 3;
          /*I have no idea what the string parameter was for there*/
          message_block list_all_messages (user) = 4;
          void delete_message(delete_message_params) = 5;
          void insert_message(insert_message_params) = 6;
        } = 1;
 } = 0x20000001;