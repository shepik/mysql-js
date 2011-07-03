SELECT execute_js("10+30");
SELECT execute_js("10+30");
SELECT execute_js("function asd(a,b) { return a+' '+b;};","asd","20","qwe");
select execute_js("function asd(a,b) { var t = b+' '+a; return t.substring(12); };","asd","20",txt) as val from test.data limit 10;
#select execute_js("find('test','data',2793)");
#select execute_js("find('test','data',27931)");
#select execute_js("for (var i=0;i<100000;i++) find('test','data',i)");
