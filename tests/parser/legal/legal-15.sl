bool b;
int abs ( int a ) {
  b = a < 0;
  if ( b ) {
    return -a;
  }
  else {
    return a;
  }
}
void main ( ) {
  abs ( -5 );
}
