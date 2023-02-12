/*
** EPITECH PROJECT, 2023
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** help_messages
*/

#ifndef HELP_MESSAGES_H_
    #define HELP_MESSAGES_H_

static const char *HELP_MESSAGE =
    "214-The following commands are recognized.\n"
    "USER PASS CWD  CDUP QUIT DELE PWD\n"
    "PASV PORT HELP NOOP RETR STOR LIST\n"
    "214 Help OK.\r\n"
;

static const char *USER_HELP_MESSAGE =
"214- USER - specify the user name for authentication. "
"The FTP server will then request the user to enter a password. "
"This command is the first step in the FTP login process.\n"
"Syntax: USER <usernamne><CRLF>\n"
"Example: USER john<CRLF>\n"
"214 End of help.\r\n";

static const char *PASS_HELP_MESSAGE =
"214- PASS - send the password for authentication after the user "
"name has been provided using the USER command.\n"
"Syntax: PASS <password><CRLF>\n"
"Example: PASS 123456<CRLF>\n"
"214 End of help.\r\n";

static const char *CWD_HELP_MESSAGE =
"214- CWD - change the current working directory on the FTP server.\n"
"Syntax: CWD <directory><CRLF>\n"
"Example: CWD /home/john/documents<CRLF>\n"
"214 End of help.\r\n";

static const char *CDUP_HELP_MESSAGE =
"214- CDUP - change the current working directory to the parent "
"directory on the FTP server.\n"
"Syntax: CDUP<CRLF>\n"
"Example: CDUP<CRLF>\n"
"214 End of help.\r\n";

static const char *QUIT_HELP_MESSAGE =
"214- QUIT - log out from the FTP server and close the connection.\n"
"Syntax: QUIT<CRLF>\n"
"Example: QUIT<CRLF>\n"
"214 End of help.\r\n";

static const char *DELE_HELP_MESSAGE =
"214- DELE - delete a file on the FTP server.\n"
"Syntax: DELE <filename><CRLF>\n"
"Example: DELE report.txt<CRLF>\n"
"214 End of help.\r\n";

static const char *PWD_HELP_MESSAGE =
"214- PWD - print the current working directory on the FTP server.\n"
"Syntax: PWD<CRLF>\n"
"Example: PWD<CRLF>\n"
"214 End of help.\r\n";

static const char *PASV_HELP_MESSAGE =
"214- PASV - switch the FTP server to passive mode. In passive mode, "
"the FTP server will wait for the client to initiate data transfers.\n"
"Syntax: PASV<CRLF>\n"
"Example: PASV<CRLF>\n"
"214 End of help.\r\n";

static const char *PORT_HELP_MESSAGE =
"214- PORT - switch the FTP server to active mode. In active mode, "
"the FTP server will initiate data transfers.\n"
"Syntax: PORT <host-port><CRLF>\n"
"Example: PORT 10.0.0.1:1234<CRLF>\n"
"214 End of help.\r\n";

static const char *HELP_HELP_MESSAGE =
"214- HELP - request help information from the FTP server.\n"
"Syntax: HELP [<comman\\r\\nd>]\n"
"Example: HELP USER<CRLF>\n"
"214 End of help.\r\n";

const char *NOOP_HELP_MESSAGE =
"214- NOOP - send a noop command to the FTP server. This command "
"is used to keep the connection alive.\n"
"Syntax: NOOP<CRLF>\n"
"Example: NOOP<CRLF>\n"
"214 End of help.\r\n";

static const char *RETR_HELP_MESSAGE =
"214- RETR - retrieve a file from the FTP server.\n"
"Syntax: RETR <filename><CRLF>\n"
"Example: RETR report.txt<CRLF>\n"
"214 End of help.\r\n";

static const char *STOR_HELP_MESSAGE =
"214- STOR - store a file on the FTP server.\n"
"Syntax: STOR <filename><CRLF>\n"
"Example: STOR report.txt<CRLF>\n"
"214 End of help.\r\n";

static const char *LIST_HELP_MESSAGE =
"214- LIST - list the contents of a directory on the FTP server.\n"
"Syntax: LIST [<directory>]<CRLF>\n"
"Example: LIST /home/john/documents<CRLF>\n"
"214 End of help.\r\n";


#endif /* !HELP_MESSAGES_H_ */
