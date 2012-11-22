s/[[:<:]][0-9]+[[:>:]]/CARDINAL_NUMBER/g
s/[[:<:]][0-9]+th[[:>:]]/ORDINAL_NUMBER/g
s/[[:<:]][0-9]*1st[[:>:]]/ORDINAL_NUMBER/g
s/[[:<:]][0-9]*2nd[[:>:]]/ORDINAL_NUMBER/g
s/[[:<:]][0-9]*3rd[[:>:]]/ORDINAL_NUMBER/g
s/[[:<:]][0-9A-Z\-\.]*[0-9][0-9A-Z\-\.]*[[:>:]]/ALPHANUM_CODE/g
