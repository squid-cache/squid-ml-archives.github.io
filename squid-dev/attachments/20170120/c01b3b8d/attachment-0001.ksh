--- squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc.manu	2017-01-18 15:41:31.021707038 +0100
+++ squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc	2017-01-20 17:41:58.321738519 +0100
@@ -205,6 +205,7 @@
     int retry = 0;
     char filter[8192];
     char searchbase[8192];
+    static const char *nulldelimiter = "";
     int rc = -1;
     if (ld) {
         if (usersearchfilter) {
@@ -267,7 +268,7 @@
             }
             value = values;
             while (*value) {
-                if (encrpass) {
+                if (encrpass && (*delimiter != *nulldelimiter)) {
                     const char *t = strtok(*value, delimiter);
                     if (t && strcmp(t, realm) == 0) {
                         password = strtok(NULL, delimiter);
@@ -577,7 +578,7 @@
     if (!userbasedn || !passattr) {
         fprintf(stderr, "Usage: " PROGRAM_NAME " -b basedn -f filter [options] ldap_server_name\n\n");
         fprintf(stderr, "\t-A password attribute(REQUIRED)\t\tUser attribute that contains the password\n");
-        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password\n");
+        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password, could be\n\t\t\t\t\t\tempty string if the password is alone in the password attribute\n");
         fprintf(stderr, "\t-b basedn (REQUIRED)\t\t\tbase dn under where to search for users\n");
         fprintf(stderr, "\t-e Encrypted passwords(REQUIRED)\tPassword are stored encrypted using HHA1\n");
         fprintf(stderr, "\t-F filter\t\t\t\tuser search filter pattern. %%s = login\n");
