#include <QApplication>
#include <QOpenGLWidget>
#include <QTimer>
#include <QOpenGLExtraFunctions>

class MiCuboWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
private:
    float angulo = 0.0f;
    QTimer *temporizador;

protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);               // Activa el buffer 3D
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Fondo gris oscuro
    }

    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double aspecto = (double)w / (double)(h ? h : 1);
        glFrustum(-aspecto, aspecto, -1.0, 1.0, 5.0, 100.0); // Perspectiva básica
        glMatrixMode(GL_MODELVIEW);
    }

    void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        glTranslatef(0.0f, 0.0f, -7.0f);    // Alejar la cámara para ver el objeto
        glRotatef(angulo, 1.0f, 1.0f, 0.5f); // Rotar en ejes X, Y y Z

        glBegin(GL_QUADS);

        // Cara frontal (Rojo)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

        // Cara trasera (Verde)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

        // Cara superior (Azul)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

        // Cara inferior (Amarillo)
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

        // Cara derecha (Cian)
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

        // Cara izquierda (Magenta)
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

        glEnd();
    }

public:
    MiCuboWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        temporizador = new QTimer(this);
        connect(temporizador, &QTimer::timeout, this, [this]() {
            angulo += 1.5f; // Velocidad de giro por frame
            update();       // Forzar redibujado de pantalla
        });
        temporizador->start(16); // ~60 FPS
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MiCuboWidget ventana;
    ventana.setWindowTitle("Cubo 3D Giratorio - Qt5 + OpenGL");
    ventana.resize(640, 480);
    ventana.show();
    
    return app.exec();
}