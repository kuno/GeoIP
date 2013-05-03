var city = new (require('./').City)('database/GeoLiteCity.dat');

for(var j = 100; --j;){
  var ip = [1,1,1,1].map(function(){return 0|Math.random()*256}).join('.');
  console.log(ip);
  city.lookup(ip, function(err, data) {
    //if (err) throw err;

    console.log(data);
  });
}
