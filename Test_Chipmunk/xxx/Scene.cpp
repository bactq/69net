#include "Precompile.h"

namespace xxx
{
    Scene::Scene()
    {
        G::scene = this;

        this->color = { 0, 0, 0, 0 };      // bg color
        this->offset = { 0, 0 };
        this->position = this->offset;

        //glClearColor( 0, 0, 0, 0 );
        //glShadeModel( GL_FLAT );
    }

    Scene::~Scene()
    {
        Node::Clear();
        G::scene = nullptr;
    }

    void Scene::Draw( int durationTicks )
    {
        if( this->dirty )
        {
            glViewport( 0, 0, ( GLsizei )this->size.width, ( GLsizei )this->size.height );
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();

            // �ȱ����� size( ��ʵ�ʲ��ֳߴ�, �̱߳���: ���̱ߵ�����Ƴߴ�̱ߣ����߱䳤 )
            if( this->designSize.width > this->designSize.height )
            {
                this->size.width = this->designSize.height * this->size.width / this->size.height;
                this->size.height = this->designSize.height;
            }
            else
            {
                this->size.height = this->designSize.width * this->size.height / this->size.width;
                this->size.width = this->designSize.width;
            }

            gluOrtho2D( 0, this->size.width, 0, this->size.height );
            glClearColor( this->color.r, this->color.g, this->color.b, this->color.a );
        }

        glClear( GL_COLOR_BUFFER_BIT );

        Node::Draw( durationTicks );

        this->dirty = false;                // resize ��ʱ������
    }
}
