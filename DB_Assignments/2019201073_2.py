#!/user/bin/env python
import sys

log_file = []
Disk_var_val = {}
def process_log_file(file_path):
    i=0
    with open(file_path,'r') as fp:
         for line in fp:
             if(line=='\n'): continue
             if(i==0): 
                j = cnt = 0
                word=""               
                while(j<len(line)):
                    if(line[j]!=' ' and line[j]!='\n'):
                       word+=line[j]
                    else:
                       if(cnt%2==0): key = word
                       else: Disk_var_val[key] = int(word)
                       word=""
                       cnt+=1
                    j+=1 
                
             else:          
                log_file.append(line.rstrip('\n'))
             i+=1

def recover_Trx():
    ed_chk_pnt = st_chk_pnt = False
    Incomp_Trx = {}
    comp_Trx = {}

    for i in range(len(log_file)-1,-1,-1):
        log_rd = log_file[i]
        if(log_rd[1:4]=='END'):
           ed_chk_pnt = True
        elif(log_rd[1:6]=='START'):
           if(len(log_rd)>11 and log_rd[7:11]=="CKPT"):
              st_chk_pnt = True
              if(ed_chk_pnt==False):
                 j=13
                 word=""
                 while(j<len(log_rd)):
                      if(log_rd[j]!=',' and log_rd[j]!=')' and log_rd[j]!='>' and log_rd[j]!=' '):
                         word+=log_rd[j]
                      elif(log_rd[j]!=' '):
                         if word not in comp_Trx:
                            Incomp_Trx[word]=True
                         word=""
                      j+=1
                 if(len(Incomp_Trx)==0):
                      break
              else:
                 break
 
           else:
              j=7
              word=""
              while(j<len(log_rd) and log_rd[j]!='>'):
                   if(log_rd[j]!=' '): 
                      word+=log_rd[j]
                   j+=1
              if word in Incomp_Trx:
                 Incomp_Trx.pop(word)
              if word not in comp_Trx:
                 comp_Trx[word]=True
                            
        elif(log_rd[1:7]=='COMMIT'):
           j=8
           word=""
           while(j<len(log_rd) and log_rd[j]!='>'):
                if(log_rd[j]!=' '): 
                   word+=log_rd[j]
                j+=1
           comp_Trx[word]=True
           if word in Incomp_Trx:
              Incomp_Trx.pop(word)
       
        else:
           var = []
           j=1
           word=""
           while(j<len(log_rd)):
                if(log_rd[j]!=',' and log_rd[j]!=' ' and log_rd[j]!='>'):
                   word+=log_rd[j]
                elif(log_rd[j]!=' '):
                     var.append(word)
                     word=""
                j+=1
           if var[0] not in comp_Trx:
              Disk_var_val[var[1]]=int(var[2])
              if var[0] not in Incomp_Trx:
                  Incomp_Trx[var[0]]=True

        if(st_chk_pnt==True and len(Incomp_Trx)==0):
           break



if __name__=="__main__":
   process_log_file(sys.argv[1])
   recover_Trx()
   with open('2019201073_2.txt','w+') as out:
        start=False
        for var in Disk_var_val:
            if(start==True):
               out.write(" ")
            out.write(var+" ")
            out.write(str(Disk_var_val[var]))
            if(start==False): start=True
