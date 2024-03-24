#include <check.h>

#include "../headers/parser.h"

obj_t obj;
int code = OK;

START_TEST(test_open_file) {
  init_obj(&obj, 1, 1);
  code = open_file("./obj/FAKE.obj", &obj);
  ck_assert_int_eq(code, FILENAME_ERROR);
  clean_obj(&obj);
}
END_TEST

START_TEST(test_parse_obj) {
  init_obj(&obj, 1, 1);
  code = open_file("./obj/Cube.obj", &obj);
  ck_assert_int_eq(obj.vertex_count, 8);
  ck_assert_float_eq_tol(obj.vertices[0], 1.745441, EPS);
  ck_assert_int_eq(obj.edge_count, 12);
  ck_assert_int_eq(obj.edges[1], 2);
  ck_assert_int_eq(code, OK);
  clean_obj(&obj);
}
END_TEST

START_TEST(test_motion) {
  init_obj(&obj, 1, 1);
  code = open_file("./obj/Cube.obj", &obj);
  move_x(&obj, -2.0);
  move_y(&obj, -3.0);
  move_z(&obj, 4.0);
  ck_assert_float_eq_tol(obj.vertices[0], -0.254559, EPS);
  ck_assert_float_eq_tol(obj.vertices[1], -1.254559, EPS);
  ck_assert_float_eq_tol(obj.vertices[2], 2.254559, EPS);
  clean_obj(&obj);
}
END_TEST

START_TEST(test_rotation) {
  init_obj(&obj, 1, 1);
  code = open_file("./obj/Cube.obj", &obj);
  rotate_x(&obj, -2.0);
  rotate_y(&obj, -3.0);
  rotate_z(&obj, 4.0);
  ck_assert_float_eq_tol(obj.vertices[0], -0.700771, EPS);
  ck_assert_float_eq_tol(obj.vertices[1], 2.7279991, EPS);
  ck_assert_float_eq_tol(obj.vertices[2], 1.0984681, EPS);
  clean_obj(&obj);
}
END_TEST

START_TEST(test_scaling) {
  init_obj(&obj, 1, 1);
  code = open_file("./obj/Cube.obj", &obj);
  scale(&obj, 2.0);
  ck_assert_float_eq_tol(obj.vertices[0], 3.490882, EPS);
  ck_assert_float_eq_tol(obj.vertices[1], 3.490882, EPS);
  ck_assert_float_eq_tol(obj.vertices[2], -3.490882, EPS);
  clean_obj(&obj);
}
END_TEST

Suite *viewer_suite(void) {
  Suite *suite;
  TCase *core;

  suite = suite_create("viewer");
  core = tcase_create("Core");

  tcase_add_test(core, test_open_file);
  tcase_add_test(core, test_parse_obj);
  tcase_add_test(core, test_motion);
  tcase_add_test(core, test_rotation);
  tcase_add_test(core, test_scaling);

  suite_add_tcase(suite, core);

  return (suite);
}

int main(void) {
  int failed = 0;
  Suite *suite;

  SRunner *runner;

  suite = viewer_suite();
  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
