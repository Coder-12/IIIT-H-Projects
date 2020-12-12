#!/usr/bin/env python
import sys

Disk_var_val = {}           # Disk variable values
Trx_oprs = {}               # Oprerations of all transaction.

def process_file(file_path):
    with open(file_path,'r') as fp:
         Trx_line_no=0
         Trx_tot_oprs=0
         Trx_id=1
         Trx_opr_no=0
         for line in fp:
             if line=='\n': continue
             if(Trx_line_no==0):
                cnt=0
                word=""
                for j in range(len(line)):
                    if line[j]!=' ' and line[j]!='\n': word+=line[j]
                    if line[j]==' ' or line[j]=='\n' or j==len(line)-1:
                       if(cnt%2==0): variable = word
                       else: Disk_var_val[variable]=int(word)
                       word=""
                       cnt+=1
             elif(Trx_line_no==1):
                cnt=0
                word=""
                for j in range(len(line)):
                    if line[j]!=' ' and line[j]!='\n': word+=line[j]
                    if line[j]==' ' or line[j]=='\n' or j==len(line)-1:
                       if(cnt==1): Trx_tot_oprs=int(word)
                       word=""
                       cnt+=1
                
             else:
                if(Trx_opr_no < Trx_tot_oprs):
                   if Trx_id not in Trx_oprs:
               	      Trx_oprs[Trx_id] = []
                   Trx_oprs[Trx_id].append(line.rstrip('\n'))
                   Trx_opr_no+=1
                   if(Trx_opr_no == Trx_tot_oprs):
                      Trx_id+=1
                      Trx_line_no = 0
                      Trx_opr_no = 0
             Trx_line_no+=1

def create_log_record(opr_quantum):
    ford_set = [' ',',','+','-','*','-',':','=','(',')','\n']
    MM_var_val = {}                           # Main_memory variable values.
    Trx_id = 1                                # Id of the transaction.
    log_rd = open("2019201073_1.txt","w+")  # Log file.
    num_of_curr_Trx=len(Trx_oprs)             # Number of cuncurrent transactions.
    processed_oprs = {}                       # Processed operation of the Trx.
    completed_Trx=0
    completed_Trx_oprs = {}                   # Completed Trx. 
    start_Trx = [True]*num_of_curr_Trx        # Indicate start of transction.

    while(1):
          if Trx_id not in processed_oprs:
             processed_oprs[Trx_id]=0
          
          i=processed_oprs[Trx_id]
          while(i < min(processed_oprs[Trx_id]+opr_quantum,len(Trx_oprs[Trx_id]))):
                oper = Trx_oprs[Trx_id][i]

                if(start_Trx[Trx_id-1]==True):
                   start_Trx[Trx_id-1]=False
                   log_rd.write("<START T"+str(Trx_id)+">\n")

                   # print main_memory variable values
                   start=False
                   for var in sorted(MM_var_val.keys()):
                       if var in Disk_var_val:
                          if(start==True): 
                             log_rd.write(" ")
                          log_rd.write(var)
                          log_rd.write(" ")
                          log_rd.write(str(MM_var_val[var]))
                          if(start==False): start=True
                   log_rd.write("\n")

                   # print Disk variable values
                   start=False
                   for var in sorted(Disk_var_val.keys()):
                       if(start==True):
                          log_rd.write(" ")
                       log_rd.write(var)
                       log_rd.write(" ")
                       log_rd.write(str(Disk_var_val[var]))
                       if(start==False): start=True            
                   log_rd.write("\n")

                if(oper[0:4] == "READ"):        # Read(A,t) -> A's value whose initial value stored in the disk copied to 't' variable.
                   j = 5
                   var1 = var2 = ""
                                    
                   while(j<len(oper) and oper[j]!=','):
                         if(ord(oper[j])>=48 and ord(oper[j])<=122):
                            var1+=oper[j]
                         j+=1

                   while(j<len(oper) and oper[j]!=')'):
                         if(ord(oper[j])>=48 and ord(oper[j])<=122):
                            var2+=oper[j]
                         j+=1
                   if var1 not in MM_var_val:
                      MM_var_val[var1]=Disk_var_val[var1] 
                   MM_var_val[var2]=MM_var_val[var1] 
                    
                elif(oper[0:5]=="WRITE"):       # write(A,t) -> t's value stored in MM copied to 'A' as updated value
                   j = 6
                   var1 = var2 = ""
                   while(j<len(oper) and oper[j]!=','):
                         if(ord(oper[j])>=48 and ord(oper[j])<=122):
                            var1+=oper[j]
                         j+=1

                   while(j<len(oper) and oper[j]!=')'):
                         if(ord(oper[j])>=48 and ord(oper[j])<=122):
                            var2+=oper[j]
                         j+=1

                   log_rd.write("<T"+str(Trx_id)+", "+var1+", "+str(MM_var_val[var1])+">\n")
                   MM_var_val[var1]=MM_var_val[var2]

                   # print Main_memory variable values
                   start=False
                   for var in sorted(MM_var_val.keys()):
                       if var in Disk_var_val:
                          if(start==True):
                             log_rd.write(" ")
                          log_rd.write(var)
                          log_rd.write(" ")
                          log_rd.write(str(MM_var_val[var]))
                          if(start==False): start=True
                   log_rd.write("\n")

                   # print Disk variable values
                   start=False
                   for var in sorted(Disk_var_val.keys()):
                       if(start==True):
                          log_rd.write(" ")
                       log_rd.write(var)
                       log_rd.write(" ")
                       log_rd.write(str(Disk_var_val[var]))
                       if(start==False): start=True
                   log_rd.write("\n")
                     
                elif(oper[0:6]=="OUTPUT"):
                   j=7
                   var = ""
                   while(j<len(oper) and oper[j]!=')'):
                       var+=oper[j]
                       j+=1
    
                   Disk_var_val[var]=MM_var_val[var]
                   if(i==len(Trx_oprs[Trx_id])-1):
                       log_rd.write("<COMMIT T"+str(Trx_id)+">\n")
                   
                       # print Main_memory variable values in the log_file
                       start=False                       
                       for var in sorted(MM_var_val.keys()):
                           if var in Disk_var_val:
                              if(start==True):
                                 log_rd.write(" ")
                              log_rd.write(var)
                              log_rd.write(" ")
                              log_rd.write(str(MM_var_val[var]))
                              if(start==False): start=True
                       log_rd.write("\n")

                       # print Disk variable values in the log_file 
                       start=False
                       for var in sorted(Disk_var_val.keys()):
                           if(start==True):
                              log_rd.write(" ")
                           log_rd.write(var) 
                           log_rd.write(" ")
                           log_rd.write(str(Disk_var_val[var]))
                           if(start==False): start=True    
                       log_rd.write("\n") 
                    
                   
                else: # operation including {/,*,+,-} need to be handled
                   #TODO
                    word = op = ""
                    var = []
                    j = 0
                    ops = ['+','-','/','*']
                    while(j<len(oper)):
                          if oper[j] not in ford_set:
                             word+=oper[j] 
                          elif(word!=""):
                             var.append(word)
                             if oper[j] in ops:
                                op=oper[j]
                             word=""
                          j+=1
                    var.append(word)
                    
                    # perform operation
                    if(op=='*'):
                       MM_var_val[var[0]] = MM_var_val[var[1]] * int(var[2])
                    elif(op=='/'):
                       if(int(var[2])==0):
                          print("Invalid operation")
                          return
                       else:
                          MM_var_val[var[0]] = MM_var_val[var[1]] / int(var[2]) 
                    elif(op=='+'):
                       MM_var_val[var[0]] = MM_var_val[var[1]] + int(var[2])
                    else:
                       MM_var_val[var[0]] = MM_var_val[var[1]] - int(var[2])    
                i+=1    

          processed_oprs[Trx_id]=i
          if(processed_oprs[Trx_id] == len(Trx_oprs[Trx_id])):
             if Trx_id not in completed_Trx_oprs:
                completed_Trx_oprs[Trx_id]=1

          if(len(completed_Trx_oprs)==num_of_curr_Trx): break
          
          Trx_id+=1
          if(Trx_id==num_of_curr_Trx+1):
             Trx_id=1
                   
                                              
if __name__ == "__main__":
   file_path = sys.argv[1] # File path of configuration file of all concurrent trx.
   opr_quantum = int(sys.argv[2]) # Operation Quantum. 
   process_file(file_path)
   create_log_record(opr_quantum)

 
