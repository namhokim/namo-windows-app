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