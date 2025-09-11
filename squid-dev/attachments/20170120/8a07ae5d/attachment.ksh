--- squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc	2017-01-18 15:41:31.021707038 +0100
+++ squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc	2017-01-18 15:54:29.553707174 +0100
@@ -63,6 +63,7 @@
 static const char *binddn = NULL;
 static const char *bindpasswd = NULL;
 static const char *delimiter = ":";
+static int notnulldelim = 0;
 static int encrpass = 0;
 static int searchscope = LDAP_SCOPE_SUBTREE;
 static int persistent = 0;
@@ -267,7 +268,7 @@
             }
             value = values;
             while (*value) {
-                if (encrpass) {
+                if (encrpass && notnulldelim) {
                     const char *t = strtok(*value, delimiter);
                     if (t && strcmp(t, realm) == 0) {
                         password = strtok(NULL, delimiter);
@@ -450,6 +451,8 @@
             break;
         case 'l':
             delimiter = value;
+            notnulldelim = strcmp(delimiter,"");
+            debug("notnulldelim = '%d'", notnulldelim);
             break;
         case 'b':
             userbasedn = value;
@@ -577,7 +580,7 @@
     if (!userbasedn || !passattr) {
         fprintf(stderr, "Usage: " PROGRAM_NAME " -b basedn -f filter [options] ldap_server_name\n\n");
         fprintf(stderr, "\t-A password attribute(REQUIRED)\t\tUser attribute that contains the password\n");
-        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password\n");
+        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password, could be\n\t\t\t\t\t\tnull if no realm in the password attribute\n");
         fprintf(stderr, "\t-b basedn (REQUIRED)\t\t\tbase dn under where to search for users\n");
         fprintf(stderr, "\t-e Encrypted passwords(REQUIRED)\tPassword are stored encrypted using HHA1\n");
         fprintf(stderr, "\t-F filter\t\t\t\tuser search filter pattern. %%s = login\n");
