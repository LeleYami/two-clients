#include "TFile.h"
#include "TServerSocket.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "Sclass.h"
#include "TROOT.h"

void hserv() 
{
   time_t nowTime1;
   time(&nowTime1);
   printf("At now time is: %s\n",ctime(&nowTime1));

   gROOT->ProcessLine(".L libSclass.so");
   TServerSocket *ss = new TServerSocket(51400, kTRUE);

   // Accept a connection and return a full-duplex communication socket.
   TSocket *s0 = ss->Accept();
   TSocket *s1 = ss->Accept();

   //tell the clients to start
   s0->Send("go 0");
   s1->Send("go 1");

   //Close the server socket
   ss->Close();

   TMonitor *mon=new TMonitor;
   
   mon->Add(s0);
   mon->Add(s1);
   
   while (1) {
       TMessage *mess;
       TSocket *s;
 
       s = mon->Select();
   
       s->Recv(mess);
 
       if(mess->What() == kMESS_STRING){
          char str[64];
          mess->ReadString(str,64);
          printf("%s\n", str);
          //printf("Client %d: %s\n", s==s0 ? 0 : 1,str);
          //mon->Remove(s);
          //if(mon->GetActive() == 0){
            //printf("No more active clients.. stopping\n");
            //break;
          //}
       }else if (mess->What() == kMESS_OBJECT){
          Sclass* m= (Sclass*)mess->ReadObject(mess->GetClass());
          m->Print();
       }

       delete mess;


       if(s == s0){
          printf("Client 1: bytes recv = %d, bytes sent = %d\n", s0->GetBytesRecv(),s0->GetBytesSent());
       }else{
          printf("Client 2: bytes recv = %d, bytes sent = %d\n", s1->GetBytesRecv(),s1->GetBytesSent());
       }


       // Close the socket.
       s->Close();

       time_t nowTime2;
       time(&nowTime2);
       printf("At now time is: %s\n",ctime(&nowTime2));
   }
}

int main(int argc, char **argv){
	hserv();
}
