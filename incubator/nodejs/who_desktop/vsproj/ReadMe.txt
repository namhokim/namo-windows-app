빌드시 주의
"포함 파일을 열 수 없습니다. 'node.h': No such file or directory"
라는 메시지가 나오는데 추가 include 디렉토리를 VS2008이 인식을 못해서 발생하는 것으로 보임
두 번째 빌드시에는 제대로 수행되므로 무시.

[test]
node
var r = require("./wts");
.exit

[wts]
> cd Release
Release> node
> var r = require("./wts");
undefined
> r.message()
TypeError: message function must be with two argument
> r.message('-1',1);
TypeError: 1st Argument must be an unsigned integer
> r.message(-1,1);
TypeError: 1st Argument must be an unsigned integer
> r.message(1,1);
TypeError: 2st Argument must be an string
> r.message(1,'1');
'OK'
> eval('('+r.enum()+')');
{ data:
   [ { SessionId: 1,
       address: '-',
       host: '-',
       rdp: 'Console' } ],
  number: 1 }
>