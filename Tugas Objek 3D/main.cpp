#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Variabel global
int windowWidth = 800;    // Lebar jendela
int windowHeight = 600;   // Tinggi jendela
int viewMode = 0;         // Mode proyeksi yang aktif
GLfloat rotation = 0.0;   // Nilai rotasi objek
GLUquadricObj *quadric;   // Untuk membuat objek quadric (silinder, disk)

// Fungsi untuk menggambar teleskop
void drawTelescope() {
  // Definisi warna dan material
  GLfloat blackMaterial[] = {0.1f, 0.1f, 0.1f, 1.0f};    // Material hitam
  GLfloat silverMaterial[] = {0.75f, 0.75f, 0.75f, 1.0f}; // Material perak
  GLfloat blueMaterial[] = {0.0f, 0.0f, 0.8f, 1.0f};     // Material biru
  GLfloat goldMaterial[] = {0.83f, 0.68f, 0.22f, 1.0f};   // Material emas
  GLfloat shininess[] = {80.0f};                         // Tingkat kilau

  // Body teleskop (warna perak)
  glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silverMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, silverMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glRotatef(90.0, 0.0, 1.0, 0.0);  // Rotasi untuk orientasi horizontal
    gluCylinder(quadric, 0.5, 0.6, 3.0, 32, 32);
  glPopMatrix();

  // Cincin lensa depan
  glPushMatrix();
    glTranslatef(3.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    
    // Cincin luar (warna emas)
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, goldMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, goldMaterial);
    gluCylinder(quadric, 0.65, 0.65, 0.1, 32, 32);  // Cincin sedikit lebih besar dari lensa
    
    // Cincin bagian dalam (kosong) di bagian depan
    glTranslatef(0.0, 0.0, 0.1);
    gluDisk(quadric, 0.6, 0.65, 32, 32);  // Membuat cincin dengan lubang tengah
  glPopMatrix();

  // Lensa depan - Menempel ke cincin
  glPushMatrix();
    glTranslatef(3.0, 0.0, 0.0);  // Pas di ujung body (body panjangnya 3.0)
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.1);  // Offset untuk posisi setelah cincin
    
    // Bagian dalam lensa (kaca) biru
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueMaterial);
    gluDisk(quadric, 0.0, 0.6, 32, 32);  // Ukuran 0.6 untuk menempel dengan body
    
    // Bagian luar lensa (warna hitam)
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blackMaterial);
    gluCylinder(quadric, 0.6, 0.6, 0.2, 32, 32);  // Lensa diatur dari titik disk
  glPopMatrix();

  // Lensa belakang / eyepiece - Menempel ke body
  glPushMatrix();
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    
    // Bagian dalam warna perak
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, silverMaterial);
    gluDisk(quadric, 0.0, 0.5, 32, 32);  // Ukuran 0.5 untuk menempel dengan body
    
    // Bagian luar lensa (warna hitam)
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blackMaterial);
    gluCylinder(quadric, 0.5, 0.4, 0.3, 32, 32);  // Lensa diatur dari titik disk
  glPopMatrix();

  // Kaki penyangga (tripod)
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blackMaterial);
  
  // Poros tengah
  glPushMatrix();
    glTranslatef(1.5, -1.8, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(quadric, 0.1, 0.1, 1.5, 16, 16);
  glPopMatrix();

  // Kaki 1
  glPushMatrix();
    glTranslatef(1.5, -1.8, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(30.0, 1.0, 0.0, 0.0);
    gluCylinder(quadric, 0.05, 0.05, 2.0, 16, 16);
  glPopMatrix();

  // Kaki 2
  glPushMatrix();
    glTranslatef(1.5, -1.8, 0.0);
    glRotatef(150.0, 0.0, 1.0, 0.0);
    glRotatef(30.0, 1.0, 0.0, 0.0);
    gluCylinder(quadric, 0.05, 0.05, 2.0, 16, 16);
  glPopMatrix();

  // Kaki 3
  glPushMatrix();
    glTranslatef(1.5, -1.8, 0.0);
    glRotatef(270.0, 0.0, 1.0, 0.0);
    glRotatef(30.0, 1.0, 0.0, 0.0);
    gluCylinder(quadric, 0.05, 0.05, 2.0, 16, 16);
  glPopMatrix();
  
  // Menggambar bayangan
  glPushMatrix();
    glTranslatef(0.0, -3.0, 0.0);
    glScalef(1.0, 0.01, 1.0);
    GLfloat shadowMaterial[] = {0.1f, 0.1f, 0.1f, 0.5f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, shadowMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shadowMaterial);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Body bayangan
    glPushMatrix();
      glRotatef(90.0, 0.0, 1.0, 0.0);
      gluCylinder(quadric, 0.5, 0.6, 3.0, 32, 32);
    glPopMatrix();
    
    // Cincin lensa depan bayangan
    glPushMatrix();
      glTranslatef(3.0, 0.0, 0.0);
      glRotatef(90.0, 0.0, 1.0, 0.0);
      gluCylinder(quadric, 0.65, 0.65, 0.1, 32, 32); 
    glPopMatrix();
    
    // Lensa depan bayangan
    glPushMatrix();
      glTranslatef(3.0, 0.0, 0.0);
      glRotatef(90.0, 0.0, 1.0, 0.0);
      glTranslatef(0.0, 0.0, 0.1);
      gluDisk(quadric, 0.0, 0.6, 32, 32);
      gluCylinder(quadric, 0.6, 0.6, 0.2, 32, 32);
    glPopMatrix();
    
    // Lensa belakang bayangan
    glPushMatrix();
      glRotatef(-90.0, 0.0, 1.0, 0.0);
      gluDisk(quadric, 0.0, 0.5, 32, 32);
      gluCylinder(quadric, 0.5, 0.4, 0.3, 32, 32);
    glPopMatrix();
    
    // Tripod bayangan (disederhanakan)
    glPushMatrix();
      glTranslatef(1.5, 0.0, 0.0);
      gluCylinder(quadric, 0.1, 0.1, 0.1, 16, 16);
    glPopMatrix();
    
    glDisable(GL_BLEND);
  glPopMatrix();
}

// Setup pencahayaan
void setupLighting() {
  GLfloat lightPosition[] = {5.0f, 5.0f, 5.0f, 1.0f};  // Posisi sumber cahaya
  GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};   // Cahaya ambient
  GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Cahaya diffuse
  GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Cahaya specular

  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);
}

// Set proyeksi berdasarkan mode yang aktif
void setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  float aspectRatio = (float)windowWidth / (float)windowHeight;
  
  switch(viewMode) {
    case 0: // Tampak atas (Orthographic)
      glOrtho(-6.0 * aspectRatio, 6.0 * aspectRatio, -6.0, 6.0, -20.0, 20.0);
      break;
    case 1: // Tampak depan (Orthographic)
      glOrtho(-6.0 * aspectRatio, 6.0 * aspectRatio, -6.0, 6.0, -20.0, 20.0);
      break;
    case 2: // Tampak samping (Orthographic)
      glOrtho(-6.0 * aspectRatio, 6.0 * aspectRatio, -6.0, 6.0, -20.0, 20.0);
      break;
    case 3: // Tampak isometrik
      glOrtho(-8.0 * aspectRatio, 8.0 * aspectRatio, -8.0, 8.0, -30.0, 30.0);
      break;
    case 4: // Tampak dimetrik
      glOrtho(-8.0 * aspectRatio, 8.0 * aspectRatio, -8.0, 8.0, -30.0, 30.0);
      break;
    case 5: // Tampak trimetrik
      glOrtho(-8.0 * aspectRatio, 8.0 * aspectRatio, -8.0, 8.0, -30.0, 30.0);
      break;
    case 6: // Perspektif (1 titik hilang)
      gluPerspective(40.0, aspectRatio, 0.1, 100.0);
      break;
    case 7: // Perspektif (2 titik hilang)
      gluPerspective(60.0, aspectRatio, 0.1, 100.0);
      break;
    case 8: // Perspektif (3 titik hilang)
      gluPerspective(75.0, aspectRatio, 0.1, 100.0);
      break;
    default:
      glOrtho(-6.0 * aspectRatio, 6.0 * aspectRatio, -6.0, 6.0, -20.0, 20.0);
  }
  
  glMatrixMode(GL_MODELVIEW);
}

// Posisikan kamera berdasarkan mode yang aktif
void positionCamera() {
  glLoadIdentity();
  
  switch(viewMode) {
    case 0: // Tampak atas
      gluLookAt(0.0, 8.0, 0.0,   0.0, -2.0, 0.0,   0.0, 0.0, -1.0);
      break;
    case 1: // Tampak depan
      gluLookAt(8.0, 0.0, 0.0,   0.0, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 2: // Tampak samping
      gluLookAt(0.0, 0.0, 8.0,   0.0, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 3: // Tampak isometrik
      gluLookAt(6.0, 6.0, 6.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 4: // Tampak dimetrik
      gluLookAt(7.0, 5.0, 7.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 5: // Tampak trimetrik
      gluLookAt(6.0, 4.0, 5.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 6: // Perspektif (1 titik hilang)
      gluLookAt(0.0, 0.0, 10.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 7: // Perspektif (2 titik hilang)
      gluLookAt(7.0, 0.0, 7.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    case 8: // Perspektif (3 titik hilang)
      gluLookAt(6.0, 5.0, 7.0,   1.5, -2.0, 0.0,   0.0, 1.0, 0.0);
      break;
    default:
      gluLookAt(0.0, 0.0, 8.0,   0.0, -2.0, 0.0,   0.0, 1.0, 0.0);
  }
  
  glRotatef(rotation, 0.0, 1.0, 0.0);
}

// Fungsi untuk menggambar grid lantai sebagai referensi
void drawGrid() {
  glDisable(GL_LIGHTING);
  glColor3f(0.5f, 0.5f, 0.5f);
  glLineWidth(1.0f);
  
  float size = 10.0f;  // Ukuran grid
  float step = 1.0f;   // Jarak antar garis
  
  glBegin(GL_LINES);
  for (float i = -size; i <= size; i += step) {
    // Garis sepanjang sumbu X
    glVertex3f(-size, -3.0f, i);
    glVertex3f(size, -3.0f, i);
    
    // Garis sepanjang sumbu Z
    glVertex3f(i, -3.0f, -size);
    glVertex3f(i, -3.0f, size);
  }
  glEnd();
  
  glEnable(GL_LIGHTING);
}

// Fungsi display utama
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  setProjection();
  positionCamera();
  setupLighting();
  
  // Gambar grid lantai untuk referensi perspektif
  drawGrid();
  
  // Gambar teleskop
  drawTelescope();
  
  // Tampilkan mode proyeksi saat ini
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 1.0);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  char *viewModeText;
  switch(viewMode) {
    case 0: viewModeText = "Tampak Atas (Orthographic)"; break;
    case 1: viewModeText = "Tampak Depan (Orthographic)"; break;
    case 2: viewModeText = "Tampak Samping (Orthographic)"; break;
    case 3: viewModeText = "Tampak Isometrik (Orthographic)"; break;
    case 4: viewModeText = "Tampak Dimetrik (Orthographic)"; break;
    case 5: viewModeText = "Tampak Trimetrik (Orthographic)"; break;
    case 6: viewModeText = "Perspektif (1 Titik Hilang)"; break;
    case 7: viewModeText = "Perspektif (2 Titik Hilang)"; break;
    case 8: viewModeText = "Perspektif (3 Titik Hilang)"; break;
    default: viewModeText = "Tampak Tidak Dikenal";
  }
  
  glRasterPos2i(10, windowHeight - 20);
  for (char *c = viewModeText; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
  }
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  glEnable(GL_LIGHTING);
  
  glutSwapBuffers();
}

// Fungsi untuk perubahan ukuran window
void reshape(int w, int h) {
  windowWidth = w;
  windowHeight = h;
  glViewport(0, 0, w, h);
  setProjection();
}

// Fungsi keyboard untuk mengubah mode proyeksi dan rotasi objek
void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case '1': viewMode = 0; break; // Tampak atas
    case '2': viewMode = 1; break; // Tampak depan
    case '3': viewMode = 2; break; // Tampak samping
    case '4': viewMode = 3; break; // Isometrik
    case '5': viewMode = 4; break; // Dimetrik
    case '6': viewMode = 5; break; // Trimetrik
    case '7': viewMode = 6; break; // Perspektif (1 titik)
    case '8': viewMode = 7; break; // Perspektif (2 titik)
    case '9': viewMode = 8; break; // Perspektif (3 titik)
    case 'r': // Rotasi objek
    case 'R':
      rotation = fmod((rotation + 5.0), 360.0);
      break;
    case 'q': // Keluar
    case 'Q':
    case 27:  // ESC
      exit(0);
      break;
  }
  glutPostRedisplay();
}

// Fungsi utama
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Teleskop 3D dengan Berbagai Proyeksi");
  
  // Buat objek quadric untuk silinder, disk, dll
  quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluQuadricNormals(quadric, GLU_SMOOTH);
  
  // Warna background
  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
  
  // Register callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  
  printf("Kontrol Program:\n");
  printf("  1 - Tampak Atas (Orthographic)\n");
  printf("  2 - Tampak Depan (Orthographic)\n");
  printf("  3 - Tampak Samping (Orthographic)\n");
  printf("  4 - Tampak Isometrik (Orthographic)\n");
  printf("  5 - Tampak Dimetrik (Orthographic)\n");
  printf("  6 - Tampak Trimetrik (Orthographic)\n");
  printf("  7 - Perspektif (1 Titik Hilang)\n");
  printf("  8 - Perspektif (2 Titik Hilang)\n");
  printf("  9 - Perspektif (3 Titik Hilang)\n");
  printf("  R - Rotasi objek\n");
  printf("  Q/ESC - Keluar\n");
  
  glutMainLoop();
  return 0;
}
