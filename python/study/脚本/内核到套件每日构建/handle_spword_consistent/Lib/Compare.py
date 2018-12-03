
f1 = open('POIXPResult.txt','r')
f2 = open('Result.txt','r')
out = open("Dif_compare_out.txt",'w+')
for line1 in f1:
    Cline1 = line1.strip()
    #print(Cline1)
    for line2 in f2:
        Cline2 = (line2.strip())
        #print(Cline2)
        if(Cline1 != Cline2):
            print('not same')
            print('label:'+Cline2)
            print('result:'+Cline1)
            out.writelines('label:'+Cline2+'\t'+'result:'+Cline1+'\n')      
        break 
out.close()
f1.close()
f2.close()

