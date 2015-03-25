mkdir dist
mkdir dist\ServerEye.Extern.API
mkdir dist\ServerEye.Extern.API\include
mkdir dist\ServerEye.Extern.API\lib
mkdir dist\ServerEye.Extern.API\lib\DebugWin32
mkdir dist\ServerEye.Extern.API\lib\Debugx64
mkdir dist\ServerEye.Extern.API\lib\ReleaseWin32
mkdir dist\ServerEye.Extern.API\lib\Releasex64

copy src\SEAPIIncludes.h dist\ServerEye.Extern.API\include\
copy src\SEAPIForwards.h dist\ServerEye.Extern.API\include\
copy src\Log.h dist\ServerEye.Extern.API\include\
copy src\SEObject.h dist\ServerEye.Extern.API\include\
copy src\Results.h dist\ServerEye.Extern.API\include\
copy src\APIConnector.h dist\ServerEye.Extern.API\include\
copy src\Container.h dist\ServerEye.Extern.API\include\
copy src\Agent.h dist\ServerEye.Extern.API\include\
copy src\AgentSetting.h dist\ServerEye.Extern.API\include\
copy src\NotifyMessage.h dist\ServerEye.Extern.API\include\
copy src\RSM.h dist\ServerEye.Extern.API\include\
copy src\PushCallBack.h dist\ServerEye.Extern.API\include\

copy Debug\ServerEye.Extern.API-v120_xp-d.lib dist\ServerEye.Extern.API\lib\DebugWin32\ServerEye.Extern.API.lib
copy lib\Debug\*.lib dist\ServerEye.Extern.API\lib\DebugWin32\
copy Debugx64\ServerEye.Extern.API-v120_xp-d.lib dist\ServerEye.Extern.API\lib\Debugx64\ServerEye.Extern.API.lib
copy lib\Debugx64\*.lib dist\ServerEye.Extern.API\lib\Debugx64\
copy Release\ServerEye.Extern.API-v120_xp.lib dist\ServerEye.Extern.API\lib\ReleaseWin32\ServerEye.Extern.API.lib
copy lib\Release\*.lib dist\ServerEye.Extern.API\lib\ReleaseWin32\
copy Releasex64\ServerEye.Extern.API-v120_xp.lib dist\ServerEye.Extern.API\lib\Releasex64\ServerEye.Extern.API.lib
copy lib\Releasex64\*.lib dist\ServerEye.Extern.API\lib\Releasex64\
pause