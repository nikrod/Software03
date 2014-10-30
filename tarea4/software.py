
# -*- coding: utf-8 -*-
import codecs

archivo=codecs.open("tarea.txt","r",encoding='utf-8')
hola=archivo.read()
archivo.close()
hola1=u""+hola

hola2=""
hola3=""
hola4=""
hola5=""
i=1
j=len(hola1)-1
k=(j/3)*2


while i<=j/3:
     hola2=hola2+hola1[i]
     i=i+1
          
while i<=k:
    hola3=hola3+hola1[i]
    i=i+1
    
while i<=j:
    hola4=hola4+hola1[i]
    i=i+1
    
                
print "\n",hola2, "\n\n",hola3,"\n\n",hola4,"\n"

i=0

while i<j/3:
    hola5=hola5+hola3[i]+hola2[i]+hola4[i]
    i=i+1
print hola5  
                   
  
 
