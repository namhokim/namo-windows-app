var index = require("./index");

function startsWith(targetString, searchString)
{
  if (searchString.length > targetString.length) {
    return false;
  }
  else {
    return ( targetString.substring(0, searchString.length) == searchString );
  }
}

function route(handle, pathname, response, request) {
  console.log("About to route a request for " + pathname);

  // static handler
  var statPath = (index.staticPath == "undefined") ? null : index.staticPath;
  var staticHandle = handle[statPath];
  if (typeof(staticHandle) === 'function' && startsWith(pathname, statPath + '/')) {
    staticHandle(response, request, pathname);
  }
  else {
    if (typeof(handle[pathname]) === 'function') {
      handle[pathname](response, request);
    } else {
      console.log("No request handler found for " + pathname);
      response.writeHead(404, {"Content-Type": "text/html"});
      response.write("404 Not found");
      response.end();
    }
  }
}

exports.route = route;