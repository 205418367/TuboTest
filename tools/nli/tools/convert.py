good = open('good_form').read().splitlines()
good = [x.split('\t') for x in good]

temp = []
for x in good:
    temp+=x

good = [x for x in temp if x!='']

bad = open('bad_form').read().splitlines()
bad = [x.split('\t') for x in bad]

temp = []
for x in bad:
    temp+=x

bad = [x for x in temp if x!='']

f = open('label_list', 'w')
for x in good:
    f.write(x+' 1\n')
for x in bad:
    f.write(x+' 0\n')
