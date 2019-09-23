wsdl2h.exe  -o SunacCADService2.h -t xsdString.bat SunacCADService.wsdl
soapcpp2.exe   .\SunacCADService2.h -i -C -L -j -I .\import -x -d .\SunacCadWeb2