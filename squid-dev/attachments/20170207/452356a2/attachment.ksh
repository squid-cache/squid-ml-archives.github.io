--- squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc.manu	2017-01-18 15:41:31.021707038 +0100
+++ squid3-3.5.23/helpers/digest_auth/LDAP/ldap_backend.cc	2017-02-07 10:29:07.026005928 +0100
@@ -63,6 +63,7 @@
 static const char *binddn = NULL;
 static const char *bindpasswd = NULL;
 static const char *delimiter = ":";
+static const char *frealm = "";
 static int encrpass = 0;
 static int searchscope = LDAP_SCOPE_SUBTREE;
 static int persistent = 0;
@@ -267,7 +268,7 @@
             }
             value = values;
             while (*value) {
-                if (encrpass) {
+                if (encrpass && *delimiter ) {
                     const char *t = strtok(*value, delimiter);
                     if (t && strcmp(t, realm) == 0) {
                         password = strtok(NULL, delimiter);
@@ -451,6 +452,9 @@
         case 'l':
             delimiter = value;
             break;
+        case 'r':
+            frealm = value;
+            break;
         case 'b':
             userbasedn = value;
             break;
@@ -574,10 +578,11 @@
     if (!ldapServer)
         ldapServer = (char *) "localhost";
 
-    if (!userbasedn || !passattr) {
-        fprintf(stderr, "Usage: " PROGRAM_NAME " -b basedn -f filter [options] ldap_server_name\n\n");
+    if (!userbasedn || !passattr || (!*delimiter && !*frealm)) {
+        fprintf(stderr, "Usage: " PROGRAM_NAME " -b basedn -F filter [options] ldap_server_name\n\n");
         fprintf(stderr, "\t-A password attribute(REQUIRED)\t\tUser attribute that contains the password\n");
-        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password\n");
+        fprintf(stderr, "\t-l password realm delimiter(REQUIRED)\tCharater(s) that devides the password attribute\n\t\t\t\t\t\tin realm and password tokens, default ':' realm:password, could be\n\t\t\t\t\t\tempty string if the password is alone in the password attribute\n");
+        fprintf(stderr, "\t-r filtered realm\t\t\tonly honor Squid requests for this realm. Mandatory if the password is alone in\n\t\t\t\t\t\tthe password attribute, acting as the inplicit realm\n");
         fprintf(stderr, "\t-b basedn (REQUIRED)\t\t\tbase dn under where to search for users\n");
         fprintf(stderr, "\t-e Encrypted passwords(REQUIRED)\tPassword are stored encrypted using HHA1\n");
         fprintf(stderr, "\t-F filter\t\t\t\tuser search filter pattern. %%s = login\n");
@@ -646,7 +651,14 @@
 {
     char *password;
     ldapconnect();
-    password = getpassword(requestData->user, requestData->realm);
+    if (*frealm)
+	if (!*delimiter && strcmp(requestData->realm, frealm))
+            password = NULL;
+        else 
+            password = getpassword(requestData->user, (char*)frealm);
+    else
+	    password = getpassword(requestData->user, requestData->realm);
+
     if (password != NULL) {
         if (encrpass)
             xstrncpy(requestData->HHA1, password, sizeof(requestData->HHA1));
@@ -660,4 +672,3 @@
     }
 
 }
-
