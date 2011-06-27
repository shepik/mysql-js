DROP FUNCTION IF EXISTS execute_js;

CREATE FUNCTION execute_js RETURNS STRING SONAME "udf_example.so";

SELECT execute_js("10+30");
