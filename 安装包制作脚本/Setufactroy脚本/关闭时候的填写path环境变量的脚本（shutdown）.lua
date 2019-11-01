resultx64 = System.Is64BitOS();


strPath = Registry.GetValue(HKEY_CURRENT_USER, "Environment", "path", true);

if strPath ~= "" then
strPath = String.Concat(strPath, ";");
end

strPathToAdd = SessionVar.Expand("%AppFolder%");

if (resultx64) then
strPathToAdd = String.Concat(strPathToAdd,"\\X64;");
else
strPathToAdd = String.Concat(strPathToAdd,"\\X86;");
end


-- 如果路径中没有这个位置加进去
if String.Find(strPath,strPathToAdd) == -1 then
strPath = String.Concat(strPath, strPathToAdd);
Registry.SetValue(HKEY_CURRENT_USER, "Environment", "path", strPath, REG_SZ);
end
  
  
-- Call this function at the end after all other On Shutdown actions.
-- This will cause the system to be rebooted, if needed.
-- Defined in _SUF70_Global_Functions.lua
-- Do not edit this script unless you know what your are doing
g_HandleSystemReboot();
