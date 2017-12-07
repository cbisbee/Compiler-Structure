import requests
APIKEY = ""
url = "http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640"

# Creating the placemarks on the canvas
url += "&markers=color:blue%7Clabel:S%7C"
url += "39.112,-108.661"
url += "|39.155,-108.735"
url += "|39.121,-108.608"
# End of the placemarks section
#Beginning of the line string section
url += "&path=color:0xff0000ff|weight:2"
url += "|39.155,-108.735"
url += "|39.130,-108.685"
url += "|39.123,-108.679"
url += "|39.117,-108.668"
url += "|39.112,-108.661"
url += "|39.110,-108.657"
url += "|39.111,-108.648"
url += "|39.114,-108.640"
url += "|39.114,-108.608"
url += "|39.121,-108.608"
#End of the line string section
#Start of polygon section
url += "&path=color%3ared|weight:1|fillcolor%3aorange"
url += "|39.1272,-108.663"
url += "|39.1116,-108.671"
url += "|39.1046,-108.639"
url += "|39.1061,-108.607"
url += "|39.12,-108.602"
url += "|39.1264,-108.607"
url += "|39.1272,-108.663"
#End of polygon section
url += "&key="
url += APIKEY
session = requests.Session()
r = session.get(url)
f = open('testMap.png','wb')
f.write(r.content)
f.close()
