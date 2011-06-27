SELECT execute_js("10+30");
SELECT execute_js("function asd(a,b) { return a+' '+b;};","asd","20","qwe");
select execute_js("function asd(a,b) { var t = b+' '+a; return t.substring(12); };","asd","20",txt) as val from test.data limit 10;
