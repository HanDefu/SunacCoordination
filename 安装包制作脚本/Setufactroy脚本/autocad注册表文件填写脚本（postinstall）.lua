result = System.Is64BitOS();
if (result) then
    --autocad2008 英文版 64
    --Registry.CreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD");
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2008X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2008 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2008X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2008 英文版 32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2008X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2008 中文版 32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2008X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	--autocad2009 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2009X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2009 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2009X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2009 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2009X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2009 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2009X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	--autocad2010 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2010X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2010 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2010X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2010 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2010X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2010 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2010X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	--autocad2011 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2011X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2011 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2011X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2011 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2011X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2011 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2011X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	--autocad2012 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2012X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2012 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2012X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2012 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2012X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2012 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2012X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	--autocad2013 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2013X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2013 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2013X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2013 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2013X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2013 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2013X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2014 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2014X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2014 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2014X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2014 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2014X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2014 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2014X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2015 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2015X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.00\\ACAD-E001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2015 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2015X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2015 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2015X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2015 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2015X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2016 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2016X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2016 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2016X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2016 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2016X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2016 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2016X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2017 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2017X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2017 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2017X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2017 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R32.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2017X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R32.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2017 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R32.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2017X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R32.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2018 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2018X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2018 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2018X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2018 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2018X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2018 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2018X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
	
	
	
	
	--autocad2019 英文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2019X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	 
    --autocad2019 中文版 64
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X64\\SunacCAD2019X64.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);

	--autocad2019 英文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2019X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	--autocad2019 中文版  32
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2019X86.arx", REG_SZ);
	Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
	
else
    --autocad2008 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2008X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2008 中文版 32 
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2008X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.1\\ACAD-6001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
     --autocad2009 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2009X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2009 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2009X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R17.2\\ACAD-7001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
     --autocad2010 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2010X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2010 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2010X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.0\\ACAD-8001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
     --autocad2011 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2011X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2011 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2011X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.1\\ACAD-9001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
     --autocad2012 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2012X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2012 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2012X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R18.2\\ACAD-A001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
    
     --autocad2013 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2013X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2013 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2013X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.0\\ACAD-B001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
    
     --autocad2014 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2014X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2014 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2014X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R19.1\\ACAD-D001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
    
     --autocad2015 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2015X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2015 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2015X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.0\\ACAD-E001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
    
     --autocad2016 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2016X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2016 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2016X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R20.1\\ACAD-F001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
     --autocad2017 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2017X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2017 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2017X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R21.0\\ACAD-0001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
     --autocad2018 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2018X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2018 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2018X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R22.0\\ACAD-1001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
    
    
    
     --autocad2019 英文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2019X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:409\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
	
    --autocad2019 中文版 32
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADER", SessionVar.Expand("%AppFolder%") .. "\\X86\\SunacCAD2019X86.arx", REG_SZ);
    Registry.SetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Autodesk\\AutoCAD\\R23.0\\ACAD-2001:804\\Applications\\SunacCAD", "LOADCTRLS", 2, REG_DWORD);
end