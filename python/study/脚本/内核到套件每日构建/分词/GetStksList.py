import json
list = []
Poi = { "list":0,"nliFieldSearch" : [ "semantic.slots.poiName", "semantic.slots.poiName2" ],"type" : "poi"}
out=open("stks.txt",'w+', encoding="utf-8")
#out=open("stks.txt",'w+')
f = open("new_input_sr.txt", 'r', encoding="utf-8")
#f = open("new_input_sr.txt", 'r')
Count=0
for eachLine in f:
    Count = Count+1
    line=eachLine.strip()
    print(line)
    dimension = []
    dimension_part = {"val": 0,"field": 0,"spword": 0}
    dimension_part["val"]=line
    dimension_part["field"]="poiName"
    dimension_part["spword"]=True
    dimension.append(dimension_part) 
    list_part = {"dimension" :0, "id" : 0}
    list_part["dimension"]=dimension
    list_part["id"]=Count
    print(list_part)
    list.append(list_part)
#print(list)
Poi["list"]=list
print(Poi)
PoiJson=json.dumps(Poi,ensure_ascii=False)
out.writelines(PoiJson)
out.close()