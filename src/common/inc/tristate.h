#include <systemc>

SC_MODULE(TRISTATE) {
    sc_core::sc_inout_rv<1> tri; // a resolved signal
    sc_core::sc_in<bool> i;
    sc_core::sc_in<bool> t;
    sc_core::sc_out<bool> o;

  void tri_method() {
    if (t.read() == 1) {
      tri.write("Z");
    } else
      tri.write(i.read());

    if (tri.read() == "z")
      o.write(1);
    else if (tri.read() == 0)
      o.write(0);
    else if (tri.read() == 1)
      o.write(1);
  }

  SC_CTOR(TRISTATE) { // init vector for possible 4-level logic values
    SC_METHOD(tri_method);
    sensitive << i << t << tri;
  }
};


