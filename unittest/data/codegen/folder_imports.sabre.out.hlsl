struct libBar_Koko {
};
libBar_Koko libBar_baz() {
	libBar_Koko _tmp_1 = {};
	return _tmp_1;
}
int libFoo_x = 0;
int libFoo_y = 12;
int libFoo_foo() {
	return libFoo_y;
}

int libBar_bar() {
	return libFoo_x + 1;
}
int libBar_bar2() {
	return libFoo_y;
}

int main_x = 234;
int main_foo() {
	libBar_Koko weijr = libBar_baz();
	return libBar_bar() + main_x + libFoo_x + libFoo_foo() + libBar_bar2();
}
