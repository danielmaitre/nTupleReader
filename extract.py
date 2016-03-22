import re
from xml.dom import minidom
xmldoc = minidom.parse('xml/classn_tuple_reader.xml')
xmldoc2 = minidom.parse('xml/class_root_file_reader_base.xml')
itemlist = xmldoc.getElementsByTagName('memberdef') 
itemlist2 = xmldoc2.getElementsByTagName('memberdef') 

#for s in itemlist :
#    print s.attributes['name'].value


functionList=[item for item in itemlist if item.attributes['kind'].value=='function']+[item for item in itemlist2 if item.attributes['kind'].value=='function']

def getTextFromNode(elem):
    children=elem.childNodes
    if len(children)==0:
        return None
    ret=[]
    if len(children)>=1:
        for child in children:
            if child.nodeType==minidom.Node.TEXT_NODE:
                ret.append(child.data)
            if child.nodeType==minidom.Node.ELEMENT_NODE and child.tagName==u'verbatim':
                ret.append(child.childNodes[0].data)
            if child.nodeType==minidom.Node.ELEMENT_NODE and child.tagName==u'ref':
                ret.append(child.childNodes[0].data)

        return ''.join(ret)

def getText(item,name):
    elems=item.getElementsByTagName(name)
    if len(elems)>0:
        return getTextFromNode(elems[0])
    else :
        return None

ii=None


excluded=['readNextEntry']

all={}

for item in functionList:
    #print item.attributes['kind'].value
    if item.attributes['kind'].value=='function':
        d={}
        for n in ['name','argsstring','type']:
            d[n]=getText(item,n)
            
        if d['name'] in excluded:
            continue
        params=item.getElementsByTagName('param')
        pars=[]    
        for par in params:
            pars.append( (getText(par,'type'),getText(par,'declname')) )


        desc=item.getElementsByTagName('detaileddescription')[0]
        paras=desc.getElementsByTagName('para')
        if len(paras)>=1:
            d['description']=getTextFromNode(paras[0]).replace('\n','').rstrip()
        argDescr=[]
        for i in range(1,len(paras)):
            argDescr.append( (pars[i-1][0],pars[i-1][1],getTextFromNode(paras[i])) )
        d['argDescr']=argDescr


        all[d['name']]=d

print all


head=r'''
\item[]
\verb|%(type)s %(name)s%(argsstring)s|\\
%(description)s'''

argTemplate=r'''\\
\null\hspace{7mm}\varref{%s} {\it %s\/}
'''

txt=''


names=all.keys()[:]

names.sort()

for name in names :
    d=all[name]    
    if d.get('description'):
        txt=txt+( head % d)
    for t,n,desc in d['argDescr']:
        print (argTemplate % (n,desc))
        txt+= (argTemplate % (n,desc))

txt=re.sub('< ','<',txt)
txt=re.sub(' >','>',txt)


f=open('members.tex','w')
f.write(txt)
