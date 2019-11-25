

///The HPClock is a portable basic clock that measures accurate time in seconds, use for profiling.
HPClock::HPClock()
{
	m_data = new HPClockData;
#ifdef B3_USE_WINDOWS_TIMERS
	QueryPerformanceFrequency(&m_data->mClockFrequency);
#endif
	reset();
}

HPClock::~HPClock()
{
	delete m_data;
}

HPClock::HPClock(const HPClock& other)
{
	m_data = new HPClockData;
	*m_data = *other.m_data;
}

HPClock& HPClock::operator=(const HPClock& other)
{
	*m_data = *other.m_data;
	return *this;
}


	/// Resets the initial reference time.
void HPClock::reset()
{
#ifdef B3_USE_WINDOWS_TIMERS
	QueryPerformanceCounter(&m_data->mStartTime);
	m_data->mStartTick = GetTickCount();
	m_data->mPrevElapsedTime = 0;
#else
#ifdef __CELLOS_LV2__

	typedef uint64_t  ClockSize;
	ClockSize newTime;
	//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
	SYS_TIMEBASE_GET( newTime );
	m_data->mStartTime = newTime;
#else
	gettimeofday(&m_data->mStartTime, 0);
#endif
#endif
}

/// Returns the time in ms since the last call to reset or since 
/// the HPClock was created.
unsigned long int HPClock::getTimeMilliseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	LONGLONG elapsedTime = currentTime.QuadPart - 
		m_data->mStartTime.QuadPart;
		// Compute the number of millisecond ticks elapsed.
	unsigned long msecTicks = (unsigned long)(1000 * elapsedTime / 
		m_data->mClockFrequency.QuadPart);
		// Check for unexpected leaps in the Win32 performance counter.  
	// (This is caused by unexpected data across the PCI to ISA 
		// bridge, aka south bridge.  See Microsoft KB274323.)
		unsigned long elapsedTicks = GetTickCount() - m_data->mStartTick;
		signed long msecOff = (signed long)(msecTicks - elapsedTicks);
		if (msecOff < -100 || msecOff > 100)
		{
			// Adjust the starting time forwards.
			LONGLONG msecAdjustment = HPClockMin(msecOff * 
				m_data->mClockFrequency.QuadPart / 1000, elapsedTime - 
				m_data->mPrevElapsedTime);
			m_data->mStartTime.QuadPart += msecAdjustment;
			elapsedTime -= msecAdjustment;

			// Recompute the number of millisecond ticks elapsed.
			msecTicks = (unsigned long)(1000 * elapsedTime / 
				m_data->mClockFrequency.QuadPart);
		}

		// Store the current elapsed time for adjustments next time.
		m_data->mPrevElapsedTime = elapsedTime;

		return msecTicks;
#else

#ifdef __CELLOS_LV2__
		uint64_t freq=sys_time_get_timebase_frequency();
		double dFreq=((double) freq) / 1000.0;
		typedef uint64_t  ClockSize;
		ClockSize newTime;
		SYS_TIMEBASE_GET( newTime );
		//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");

		return (unsigned long int)((double(newTime-m_data->mStartTime)) / dFreq);
#else

		struct timeval currentTime;
		gettimeofday(&currentTime, 0);
		return (currentTime.tv_sec - m_data->mStartTime.tv_sec) * 1000 + 
			(currentTime.tv_usec - m_data->mStartTime.tv_usec) / 1000;
#endif //__CELLOS_LV2__
#endif
}

	/// Returns the time in us since the last call to reset or since 
	/// the Clock was created.
unsigned long int HPClock::getTimeMicroseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		LONGLONG elapsedTime = currentTime.QuadPart - 
			m_data->mStartTime.QuadPart;

		// Compute the number of millisecond ticks elapsed.
		unsigned long msecTicks = (unsigned long)(1000 * elapsedTime / 
			m_data->mClockFrequency.QuadPart);

		// Check for unexpected leaps in the Win32 performance counter.  
		// (This is caused by unexpected data across the PCI to ISA 
		// bridge, aka south bridge.  See Microsoft KB274323.)
		unsigned long elapsedTicks = GetTickCount() - m_data->mStartTick;
		signed long msecOff = (signed long)(msecTicks - elapsedTicks);
		if (msecOff < -100 || msecOff > 100)
		{
			// Adjust the starting time forwards.
			LONGLONG msecAdjustment = HPClockMin(msecOff * 
				m_data->mClockFrequency.QuadPart / 1000, elapsedTime - 
				m_data->mPrevElapsedTime);
			m_data->mStartTime.QuadPart += msecAdjustment;
			elapsedTime -= msecAdjustment;
		}

		// Store the current elapsed time for adjustments next time.
		m_data->mPrevElapsedTime = elapsedTime;

		// Convert to microseconds.
		unsigned long usecTicks = (unsigned long)(1000000 * elapsedTime / 
			m_data->mClockFrequency.QuadPart);

		return usecTicks;
#else

#ifdef __CELLOS_LV2__
		uint64_t freq=sys_time_get_timebase_frequency();
		double dFreq=((double) freq)/ 1000000.0;
		typedef uint64_t  ClockSize;
		ClockSize newTime;
		//__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
		SYS_TIMEBASE_GET( newTime );

		return (unsigned long int)((double(newTime-m_data->mStartTime)) / dFreq);
#else

		struct timeval currentTime;
		gettimeofday(&currentTime, 0);
		return (currentTime.tv_sec - m_data->mStartTime.tv_sec) * 1000000 + 
			(currentTime.tv_usec - m_data->mStartTime.tv_usec);
#endif//__CELLOS_LV2__
#endif 
}

GlDrawcallback::GlDrawcallback()
		:m_wireframe(false)
	{
	}

	void GlDrawcallback::processTriangle(btVector3* triangle, int partId, int triangleIndex)
	{

		// (void)triangleIndex;
		// (void)partId;


		if (m_wireframe)
		{
			// glBegin(GL_LINES);
			// //glColor3f(1, 0, 0);
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// //glColor3f(0, 1, 0);
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// // /glColor3f(0, 0, 1);
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			// glEnd();
		} else
		{
			//glBegin(GL_TRIANGLES);
			//glColor3f(1, 1, 1);
			
			//cout << "f\n";
			
			
			btVector3 normal = (triangle[2]-triangle[0]).cross(triangle[1]-triangle[0]);
			normal.normalize();
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			
			glVertex3f(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3f(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3f(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			
			// cout << triangle[0].getX() << " " << triangle[0].getY() << " " << triangle[0].getZ() << "\n";
			// cout << triangle[1].getX() << " " << triangle[1].getY() << " " << triangle[1].getZ() << "\n";
			// cout << triangle[2].getX() << " " << triangle[2].getY() << " " << triangle[2].getZ() << "\n";
			// cout << normal.getX() << " " << normal.getY() << " " << normal.getZ() << "\n";
			// cout << "\n";
			
			// normal = (triangle[0]-triangle[2]).cross(triangle[1]-triangle[2]);
			// normal.normalize();
			// glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			
			// glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			// glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			// glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			//glEnd();
		}
	}

void TriangleGlDrawcallback::internalProcessTriangleIndex(btVector3* triangle,int partId,int  triangleIndex)
	{
		(void)triangleIndex;
		(void)partId;



		//glBegin(GL_TRIANGLES);//LINES);
		//glColor3f(1, 0, 0);
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		//glColor3f(0, 1, 0);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
		// /glColor3f(0, 0, 1);
		glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
		glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
		//glEnd();
	}







OGLRenderer::OGLRenderer(int width, int height)
 :m_width(width),
 m_height(height)
{
    
}

void OGLRenderer::init()
{
}

const CommonCameraInterface* OGLRenderer::getActiveCamera() const
{
	return &m_camera;
}
CommonCameraInterface* OGLRenderer::getActiveCamera()
{
	return &m_camera;
}
void OGLRenderer::setActiveCamera(CommonCameraInterface* cam)
{
	b3Assert(0);//not supported yet
}

void OGLRenderer::updateCamera(int upAxis)
{
    float projection[16];
    float view[16];
    m_camera.setAspectRatio((float)m_width/(float)m_height);
    m_camera.update();
    m_camera.getCameraProjectionMatrix(projection);
    m_camera.getCameraViewMatrix(view);
    GLfloat projMat[16];
    GLfloat viewMat[16];
    for (int i=0;i<16;i++)
    {
        viewMat[i] = view[i];
        projMat[i] = projection[i];
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(projMat);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(viewMat);
}

void OGLRenderer::removeAllInstances()
{
}


void OGLRenderer::writeSingleInstanceColorToCPU(float* color, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceColorToCPU(double* color, int srcIndex)
{
    
}
void OGLRenderer::getCameraViewMatrix(float viewMat[16]) const
{
    b3Assert(0);
}
void OGLRenderer::getCameraProjectionMatrix(float projMat[16]) const
{
    b3Assert(0);
    
}


void OGLRenderer::renderScene()
{
    //cout << "asdfasdf\n";
}
    
    


int OGLRenderer::registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling)
{
    return 0;
}

int OGLRenderer::registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling)
{
    return 0;
}

void OGLRenderer::drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize)
{
    int pointStrideInFloats = pointStrideInBytes/4;
    for (int i=0;i<numIndices;i+=2)
    {
        int index0 = indices[i];
        int index1 = indices[i+1];
        
        btVector3 fromColor = b3MakeVector3(color[0],color[1],color[2]);
        btVector3 toColor = b3MakeVector3(color[0],color[1],color[2]);
        
        btVector3 from= b3MakeVector3(positions[index0*pointStrideInFloats],positions[index0*pointStrideInFloats+1],positions[index0*pointStrideInFloats+2]);
        btVector3 to= b3MakeVector3(positions[index1*pointStrideInFloats],positions[index1*pointStrideInFloats+1],positions[index1*pointStrideInFloats+2]);
        
        // glBegin(GL_LINES);
        // glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
        // glVertex3d(from.getX(), from.getY(), from.getZ());
        // glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
        // glVertex3d(to.getX(), to.getY(), to.getZ());
        // glEnd();
        
    }
}

void OGLRenderer::drawLine(const float from[4], const float to[4], const float color[4], float lineWidth)
{
        // glBegin(GL_LINES);
        // glColor3f(color[0],color[1],color[2]);
        // glVertex3d(from[0],from[1],from[2]);
        // glVertex3d(to[0],to[1],to[2]);
        // glEnd();
}
int OGLRenderer::registerShape(const float* vertices, int numvertices, const int* indices, int numIndices,int primitiveType, int textureIndex)
{
    return 0;
}

void OGLRenderer::writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex)
{
}
void OGLRenderer::writeTransforms()
{
}


void OGLRenderer::drawLine(const double from[4], const double to[4], const double color[4], double lineWidth)
{
    
}
void OGLRenderer::drawPoint(const float* position, const float color[4], float pointDrawSize)
{
}
void OGLRenderer::drawPoint(const double* position, const double color[4], double pointDrawSize)
{
}

void OGLRenderer::updateShape(int shapeIndex, const float* vertices)
{
}

void OGLRenderer::enableBlend(bool blend)
{
}

MyDebugDrawer::MyDebugDrawer(CommonGraphicsApp* app)
		: m_glApp(app)
		,m_debugMode(btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawAabb),
		m_currentLineColor(-1,-1,-1)
	{
		
		
	}
	 DefaultColors	MyDebugDrawer::getDefaultColors() const
	{	
		return m_ourColors;
	}
	///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
	void MyDebugDrawer::setDefaultColors(const DefaultColors& colors)
	{
		m_ourColors = colors;
	}


	void MyDebugDrawer::drawLine(const btVector3& from1,const btVector3& to1,const btVector3& color1)
	{
        //float from[4] = {from1[0],from1[1],from1[2],from1[3]};
        //float to[4] = {to1[0],to1[1],to1[2],to1[3]};
        //float color[4] = {color1[0],color1[1],color1[2],color1[3]};
		//m_glApp->m_instancingRenderer->drawLine(from,to,color);
		if (m_currentLineColor!=color1 || m_linePoints.size() >= BT_LINE_BATCH_SIZE)
        {
            flushLines();
            m_currentLineColor = color1;
        }
		MyDebugVec3 from(from1);
		MyDebugVec3 to(to1);
			
		m_linePoints.push_back(from);
		m_linePoints.push_back(to);

		m_lineIndices.push_back(m_lineIndices.size());
		m_lineIndices.push_back(m_lineIndices.size());

	}

	void MyDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{
        drawLine(PointOnB,PointOnB+normalOnB,color);
	}
     

	void MyDebugDrawer::reportErrorWarning(const char* warningString)
	{
	}

	void MyDebugDrawer::draw3dText(const btVector3& location,const char* textString)
	{
	}

	void MyDebugDrawer::setDebugMode(int debugMode)
	{
		m_debugMode = debugMode;
	}

	int		MyDebugDrawer::getDebugMode() const
	{
		return m_debugMode;
	}

    void MyDebugDrawer::flushLines()
	{
	    int sz = m_linePoints.size();
	    if (sz)
        {
			float debugColor[4];
		debugColor[0] = m_currentLineColor.x();
		debugColor[1] = m_currentLineColor.y();
		debugColor[2] = m_currentLineColor.z();
		debugColor[3] = 1.f;
		m_glApp->m_renderer->drawLines(&m_linePoints[0].x,debugColor,
														 m_linePoints.size(),sizeof(MyDebugVec3),
														 &m_lineIndices[0],
														 m_lineIndices.size(),
														 1);
            m_linePoints.clear();
            m_lineIndices.clear();
        }
	}


bool shapePointerCompareFunc(const btCollisionObject* colA, const btCollisionObject* colB)
{
	MyConvertPointerSizeT a,b;
	a.m_ptr = colA->getCollisionShape();
	b.m_ptr = colB->getCollisionShape();
	return (a.m_int<b.m_int);
}

struct MyGLHelperInternalData
{
	struct CommonGraphicsApp* m_glApp;
	class MyDebugDrawer* m_debugDraw;
	MyShapeDrawer* m_gl2ShapeDrawer;
};




	
	// return true when pairs need collision
	bool CustFilterCallback::needBroadphaseCollision(
		btBroadphaseProxy* proxy0,
		btBroadphaseProxy* proxy1) const
	{
		
		if (EDIT_POSE) {
			return false;
		}
		
		bool collides = 
		(
			((proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0) &&
			((proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask) != 0)
		);


		
		btCollisionObject* colObj0 = (btCollisionObject*)(proxy0->m_clientObject);
		btCollisionObject* colObj1 = (btCollisionObject*)(proxy1->m_clientObject);
		
		
		
		// if (colObj0->bodyUID != colObj1->bodyUID) {
		// 	cout << colObj0->bodyUID << " " << colObj1->bodyUID << "\n";
		// }
		
		collides = collides && (
			(colObj0->bodyUID != colObj1->bodyUID) &&
			(colObj0->bodyUID > -2) &&
			(colObj1->bodyUID > -2) &&
			(colObj0->heldByUID != colObj1->bodyUID) &&
			(colObj1->heldByUID != colObj0->bodyUID)
		);
		
		
		
		return collides;
	}


    

    btRaycastBar2::btRaycastBar2 ()
	{
		m_guiHelper = 0;
		ms = 0;
		max_ms = 0;
		min_ms = 9999;
		sum_ms_samples = 0;
		sum_ms = 0;
	}



    btRaycastBar2::btRaycastBar2 (btScalar ray_length, btScalar z,btScalar max_y,struct GUIHelperInterface* guiHelper)
	{
		m_guiHelper = guiHelper;
		frame_counter = 0;
		ms = 0;
		max_ms = 0;
		min_ms = 9999;
		sum_ms_samples = 0;
		sum_ms = 0;
		dx = 10.0;
		min_x = 0;
		max_x = 0;
		this->max_y = max_y;
		sign = 1.0;
		btScalar dalpha = 2*SIMD_2_PI/NUMRAYS;
		for (int i = 0; i < NUMRAYS; i++)
		{
			btScalar alpha = dalpha * i;
			// rotate around by alpha degrees y 
			btQuaternion q(btVector3(0.0, 1.0, 0.0), alpha);
			direction[i] = btVector3(1.0, 0.0, 0.0);
			direction[i] = quatRotate(q , direction[i]);
			direction[i] = direction[i] * ray_length;
			
			
			source[i] = btVector3(min_x, max_y, z);
			dest[i] = source[i] + direction[i];
			dest[i][1]=-1000;
			normal[i] = btVector3(1.0, 0.0, 0.0);
		}
	}

	void btRaycastBar2::move (btScalar dt)
	{
		if (dt > btScalar(1.0/60.0))
			dt = btScalar(1.0/60.0);
		for (int i = 0; i < NUMRAYS; i++)
		{
			source[i][0] += dx * dt * sign;
			dest[i][0] += dx * dt * sign;
		}
		if (source[0][0] < min_x)
			sign = 1.0;
		else if (source[0][0] > max_x)
			sign = -1.0;
	}

	void btRaycastBar2::cast (btCollisionWorld* cw)
	{
#ifdef USE_BT_CLOCK
		frame_timer.reset ();
#endif //USE_BT_CLOCK

#ifdef BATCH_RAYCASTER
		if (!gBatchRaycaster)
			return;

		gBatchRaycaster->clearRays ();
		for (int i = 0; i < NUMRAYS; i++)
		{
			gBatchRaycaster->addRay (source[i], dest[i]);
		}
		gBatchRaycaster->performBatchRaycast ();
		for (int i = 0; i < gBatchRaycaster->getNumRays (); i++)
		{
				const SpuRaycastTaskWorkUnitOut& out = (*gBatchRaycaster)[i];
				hit[i].setInterpolate3(source[i],dest[i],out.hitFraction);
				normal[i] = out.hitNormal;
				normal[i].normalize ();
		}
#else
		for (int i = 0; i < NUMRAYS; i++)
		{
			btCollisionWorld::ClosestRayResultCallback cb(source[i], dest[i]);
			
			cw->rayTest (source[i], dest[i], cb);
			if (cb.hasHit ())
			{
				hit[i] = cb.m_hitPointWorld;
				normal[i] = cb.m_hitNormalWorld;
				normal[i].normalize ();
			} else {
				hit[i] = dest[i];
				normal[i] = btVector3(1.0, 0.0, 0.0);
			}

		}
#ifdef USE_BT_CLOCK
		ms += frame_timer.getTimeMilliseconds ();
#endif //USE_BT_CLOCK
		frame_counter++;
		if (frame_counter > 50)
		{
			min_ms = ms < min_ms ? ms : min_ms;
			max_ms = ms > max_ms ? ms : max_ms;
			sum_ms += ms;
			sum_ms_samples++;
			btScalar mean_ms = (btScalar)sum_ms/(btScalar)sum_ms_samples;
			printf("%d rays in %d ms %d %d %f\n", NUMRAYS * frame_counter, ms, min_ms, max_ms, mean_ms);
			ms = 0;
			frame_counter = 0;
		}
#endif
	}

	void btRaycastBar2::draw ()
	{
		
		if (m_guiHelper)
		{
			btAlignedObjectArray<unsigned int> indices;
			btAlignedObjectArray<btVector3FloatData> points;
			
			
			float lineColor[4]={1,0.4,.4,1};
			
			for (int i = 0; i < NUMRAYS; i++)
			{
				btVector3FloatData s,h;
				for (int w=0;w<4;w++)
				{
					s.m_floats[w] = source[i][w];
					h.m_floats[w] = hit[i][w];
				}
				
				points.push_back(s);
				points.push_back(h);
				indices.push_back(indices.size());
				indices.push_back(indices.size());
			}

			m_guiHelper->getRenderInterface()->drawLines(&points[0].m_floats[0],lineColor,points.size(),sizeof(btVector3),&indices[0],indices.size(),1);
		}
													 
#if 0
		glDisable (GL_LIGHTING);
		glColor3f (0.0, 1.0, 0.0);
		glBegin (GL_LINES);
		int i;

		for (i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (source[i][0], source[i][1], source[i][2]);
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
		}
		glEnd ();
		glColor3f (1.0, 1.0, 1.0);
		glBegin (GL_LINES);
		for (i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
			glVertex3f (hit[i][0] + normal[i][0], hit[i][1] + normal[i][1], hit[i][2] + normal[i][2]);
		}
		glEnd ();
		glColor3f (0.0, 1.0, 1.0);
		glBegin (GL_POINTS);
		for ( i = 0; i < NUMRAYS; i++)
		{
			glVertex3f (hit[i][0], hit[i][1], hit[i][2]);
		}
		glEnd ();
		glEnable (GL_LIGHTING);
#endif //USE_GRAPHICAL_BENCHMARK

	}
};


btDiscreteDynamicsWorld* BenchmarkDemo::getWorld() {
	return m_dynamicsWorld;
}

void BenchmarkDemo::stepSimulation(float deltaTime)
{
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(deltaTime);
	}
	
	if (m_benchmark==7)
	{
		castRays();

		raycastBar.draw();
	
	}

}

void BenchmarkDemo::removeRigidBody(btRigidBody* body) {
	m_dynamicsWorld->removeRigidBody(body);
}


btRigidBody* BenchmarkDemo::createRigidBodyMask(
	btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
) {

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic) {
		shape->calculateLocalInertia(mass,localInertia);
	}
		

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world 
	m_dynamicsWorld->addRigidBody(body, maskFrom, maskTo); //
	
	return body;
}


void BenchmarkDemo::updateGraphicsObjects() {
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}


void BenchmarkDemo::beginDrop(float x, float y, float z) {
	initOffset = btVector3(x,y,z);
	createTest5();
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void BenchmarkDemo::initPhysics()
{
	cout << "BenchmarkDemo:initPhysics()\n";
	
	
	m_guiHelper->setUpAxis(2);

	setCameraDistance(btScalar(100.));

	///collision configuration contains default setup for memory, collision setup
	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
	m_collisionConfiguration = new btDefaultCollisionConfiguration(cci);

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	
	m_dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);



	///the maximum size of the collision world. Make sure objects stay within these boundaries
	///Don't make the world AABB size too large, it will harm simulation quality and performance
	btVector3 worldAabbMin(-16384,-16384,-16384);
	btVector3 worldAabbMax(16384,16384,16384);
	
//	btHashedOverlappingPairCache* pairCache = new btHashedOverlappingPairCache();
//	m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,3500,pairCache);
//	m_broadphase = new btSimpleBroadphase();
	m_broadphase = new btDbvtBroadphase();
	

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	
	
	m_solver = sol;

	btDiscreteDynamicsWorld* dynamicsWorld;
	m_dynamicsWorld = dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	

	///the following 3 lines increase the performance dramatically, with a little bit of loss of quality
	m_dynamicsWorld->getSolverInfo().m_solverMode |=SOLVER_ENABLE_FRICTION_DIRECTION_CACHING; //don't recalculate friction values each frame
	dynamicsWorld->getSolverInfo().m_numIterations = 5; //few solver iterations 
	//m_defaultContactProcessingThreshold = 0.f;//used when creating bodies: body->setContactProcessingThreshold(...);
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	

	m_dynamicsWorld->setGravity(btVector3(0,0,-10));
	
	
	
	

	// if (m_benchmark<5)
	// {
	// 	///create a few basic rigid bodies
	// 	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(250.),btScalar(250.),btScalar(50.)));
	// //	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0);
		
	// 	m_collisionShapes.push_back(groundShape);

	// 	btTransform groundTransform;
	// 	groundTransform.setIdentity();
	// 	groundTransform.setOrigin(btVector3(0,0,-50));

	// 	//We can also use DemoApplication::createRigidBody, but for clarity it is provided here:
	// 	{
	// 		btScalar mass(0.);

	// 		//rigidbody is dynamic if and only if mass is non zero, otherwise static
	// 		bool isDynamic = (mass != 0.f);

	// 		btVector3 localInertia(0,0,0);
	// 		if (isDynamic)
	// 			groundShape->calculateLocalInertia(mass,localInertia);

	// 		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// 		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	// 		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	// 		btRigidBody* body = new btRigidBody(rbInfo);

	// 		//add the body to the dynamics world
	// 		m_dynamicsWorld->addRigidBody(body);
	// 	}
	// }

	// switch (m_benchmark)
	// {
	// 	case 1:
	// 		{
	// 			createTest1();
	// 			break;
	// 		}
	// 	case 2:
	// 		{
	// 			createTest2();
	// 			break;
	// 		}
	// 	case 3:
	// 		{
	// 			createTest3();
	// 			break;
	// 		}
	// 	case 4:
	// 		{
	// 			createTest4();
	// 			break;
	// 		}
	// 	case 5:
	// 		{
	// 			createTest5();
	// 			break;
	// 		}
	// 	case 6:
	// 	{
	// 		createTest6();
	// 		break;
	// 	}
	// 	case 7:
	// 	{
	// 		createTest7();
	// 		break;
	// 	}
	// 	case 8:
	// 	{
	// 		createTest8();
	// 		break;
	// 	}


	// default:
	// 	{
	// 	}			
	// }

	

}
	

void BenchmarkDemo::createTest1()
{
	// 3000
	int size = 8;
	const float cubeSize = 1.0f;
	float spacing = cubeSize;
	btVector3 pos(0.0f, cubeSize * 2,0.f);
	float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;

	btBoxShape* blockShape = new btBoxShape(btVector3(cubeSize-COLLISION_RADIUS,cubeSize-COLLISION_RADIUS,cubeSize-COLLISION_RADIUS));
	btVector3 localInertia(0,0,0);
	float mass = 2.f;
	blockShape->calculateLocalInertia(mass,localInertia);
	
	btTransform trans;
	trans.setIdentity();

	for(int k=0;k<47;k++) {
		for(int j=0;j<size;j++) {
			pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
			for(int i=0;i<size;i++) {
				pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);

				trans.setOrigin(pos);
				btRigidBody* cmbody;
				cmbody= createRigidBody(mass,trans,blockShape);
			}
		}
		offset -= 0.05f * spacing * (size-1);
//		spacing *= 1.01f;
		pos[1] += (cubeSize * 2.0f + spacing);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Pyramid 3

void BenchmarkDemo::createWall(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize)
{
	
	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);

//	btScalar  diffX = boxSize[0] * 1.0f;
	btScalar  diffY = boxSize[1] * 1.0f;
	btScalar  diffZ = boxSize[2] * 1.0f;

	btScalar  offset = -stackSize * (diffZ * 2.0f) * 0.5f;
	btVector3 pos(0.0f, diffY, 0.0f);

	btTransform trans;
	trans.setIdentity();

	while(stackSize) {
		for(int i=0;i<stackSize;i++) {
			pos[2] = offset + (float)i * (diffZ * 2.0f);

		trans.setOrigin(offsetPosition + pos);
		createRigidBody(mass,trans,blockShape);
		
		}
		offset += diffZ;
		pos[1] += (diffY * 2.0f);
		stackSize--;
	}
}

void BenchmarkDemo::createPyramid(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize)
{
	btScalar space = 0.0001f;
	
	btVector3 pos(0.0f, boxSize[1], 0.0f);

	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));
	btTransform trans;
	trans.setIdentity();

	btScalar mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);

	
	btScalar diffX = boxSize[0]*1.02f;
	btScalar diffY = boxSize[1]*1.02f;
	btScalar diffZ = boxSize[2]*1.02f;
	
	btScalar offsetX = -stackSize * (diffX * 2.0f + space) * 0.5f;
	btScalar offsetZ = -stackSize * (diffZ * 2.0f + space) * 0.5f;
	while(stackSize) {
		for(int j=0;j<stackSize;j++) {
			pos[2] = offsetZ + (float)j * (diffZ * 2.0f + space);
			for(int i=0;i<stackSize;i++) {
				pos[0] = offsetX + (float)i * (diffX * 2.0f + space);
				trans.setOrigin(offsetPosition + pos);
				this->createRigidBody(mass,trans,blockShape);


			}
		}
		offsetX += diffX;
		offsetZ += diffZ;
		pos[1] += (diffY * 2.0f + space);
		stackSize--;
	}

}

 const btVector3 rotate( const btQuaternion& quat, const btVector3 & vec ) {
    float tmpX, tmpY, tmpZ, tmpW;
    tmpX = ( ( ( quat.getW() * vec.getX() ) + ( quat.getY() * vec.getZ() ) ) - ( quat.getZ() * vec.getY() ) );
    tmpY = ( ( ( quat.getW() * vec.getY() ) + ( quat.getZ() * vec.getX() ) ) - ( quat.getX() * vec.getZ() ) );
    tmpZ = ( ( ( quat.getW() * vec.getZ() ) + ( quat.getX() * vec.getY() ) ) - ( quat.getY() * vec.getX() ) );
    tmpW = ( ( ( quat.getX() * vec.getX() ) + ( quat.getY() * vec.getY() ) ) + ( quat.getZ() * vec.getZ() ) );
    return btVector3(
        ( ( ( ( tmpW * quat.getX() ) + ( tmpX * quat.getW() ) ) - ( tmpY * quat.getZ() ) ) + ( tmpZ * quat.getY() ) ),
        ( ( ( ( tmpW * quat.getY() ) + ( tmpY * quat.getW() ) ) - ( tmpZ * quat.getX() ) ) + ( tmpX * quat.getZ() ) ),
        ( ( ( ( tmpW * quat.getZ() ) + ( tmpZ * quat.getW() ) ) - ( tmpX * quat.getY() ) ) + ( tmpY * quat.getX() ) )
    );
}

void BenchmarkDemo::createTowerCircle(const btVector3& offsetPosition,int stackSize,int rotSize,const btVector3& boxSize)
{

	btBoxShape* blockShape = new btBoxShape(btVector3(boxSize[0]-COLLISION_RADIUS,boxSize[1]-COLLISION_RADIUS,boxSize[2]-COLLISION_RADIUS));

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	blockShape->calculateLocalInertia(mass,localInertia);


	float radius = 1.3f * rotSize * boxSize[0] / SIMD_PI;

	// create active boxes
	btQuaternion rotY(0,1,0,0);
	float posY = boxSize[1];

	for(int i=0;i<stackSize;i++) {
		for(int j=0;j<rotSize;j++) {
		

			trans.setOrigin(offsetPosition+  rotate(rotY,btVector3(0.0f , posY, radius)));
			trans.setRotation(rotY);
			createRigidBody(mass,trans,blockShape);

			rotY *= btQuaternion(btVector3(0,1,0),SIMD_PI/(rotSize*btScalar(0.5)));
		}

		posY += boxSize[1] * 2.0f;
		rotY *= btQuaternion(btVector3(0,1,0),SIMD_PI/(float)rotSize);
	}
	
}

void BenchmarkDemo::createTest2()
{
	setCameraDistance(btScalar(50.));
	const float cubeSize = 1.0f;

	createPyramid(btVector3(-20.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(-2.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(4.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createWall(btVector3(10.0f,0.0f,0.0f),12,btVector3(cubeSize,cubeSize,cubeSize));
	createTowerCircle(btVector3(25.0f,0.0f,0.0f),8,24,btVector3(cubeSize,cubeSize,cubeSize));
	
}






	btRigidBody* RagDoll::createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic) {
			shape->calculateLocalInertia(mass,localInertia);
		}
		

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_ownerWorld->addRigidBody(body);

		return body;
	}

    RagDoll::RagDoll (btDynamicsWorld* ownerWorld, const btVector3& positionOffset,btScalar scale)
		: m_ownerWorld (ownerWorld)
	{
		// Setup the geometry
		m_shapes[BODYPART_PELVIS] = new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.20)*scale);
		m_shapes[BODYPART_SPINE] = new btCapsuleShape(btScalar(0.15)*scale, btScalar(0.28)*scale);
		m_shapes[BODYPART_HEAD] = new btCapsuleShape(btScalar(0.10)*scale, btScalar(0.05)*scale);
		m_shapes[BODYPART_LEFT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
		m_shapes[BODYPART_LEFT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
		m_shapes[BODYPART_RIGHT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07)*scale, btScalar(0.45)*scale);
		m_shapes[BODYPART_RIGHT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.37)*scale);
		m_shapes[BODYPART_LEFT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
		m_shapes[BODYPART_LEFT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);
		m_shapes[BODYPART_RIGHT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05)*scale, btScalar(0.33)*scale);
		m_shapes[BODYPART_RIGHT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04)*scale, btScalar(0.25)*scale);

		// Setup all the rigid bodies
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
		m_bodies[BODYPART_PELVIS] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_PELVIS]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
		m_bodies[BODYPART_SPINE] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_SPINE]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
		m_bodies[BODYPART_HEAD] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_HEAD]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_LEFT_UPPER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_LEFT_LOWER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_UPPER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_LOWER_LEG] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_UPPER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_LOWER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_UPPER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(scale*btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_LOWER_ARM] = createRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_ARM]);

		// Setup some damping on the m_bodies
		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			m_bodies[i]->setDamping(btScalar(0.05), btScalar(0.85));
			m_bodies[i]->setDeactivationTime(btScalar(0.8));
			m_bodies[i]->setSleepingThresholds(btScalar(1.6), btScalar(2.5));
		}

		// Now setup the constraints
		btHingeConstraint* hingeC;
		btConeTwistConstraint* coneC;

		btTransform localA, localB;

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_SPINE], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
		m_joints[JOINT_PELVIS_SPINE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_PELVIS_SPINE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_2); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.30), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_HEAD], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, M_PI_2);
		m_joints[JOINT_SPINE_HEAD] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_SPINE_HEAD], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,-M_PI_4*5); localA.setOrigin(scale*btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,-M_PI_4*5); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_LEFT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_LEFT_HIP] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_LEG], *m_bodies[BODYPART_LEFT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_LEFT_KNEE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_4); localA.setOrigin(scale*btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_4); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_RIGHT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_RIGHT_HIP] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_LEG], *m_bodies[BODYPART_RIGHT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_RIGHT_KNEE] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI); localA.setOrigin(scale*btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_LEFT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		m_joints[JOINT_LEFT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_ARM], *m_bodies[BODYPART_LEFT_LOWER_ARM], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		m_joints[JOINT_LEFT_ELBOW] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_ELBOW], true);



		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,0); localA.setOrigin(scale*btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_RIGHT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		m_joints[JOINT_RIGHT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(scale*btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(scale*btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_ARM], *m_bodies[BODYPART_RIGHT_LOWER_ARM], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		m_joints[JOINT_RIGHT_ELBOW] = hingeC;
		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_ELBOW], true);
	}

	RagDoll::~RagDoll ()
	{
		int i;

		// Remove all constraints
		for ( i = 0; i < JOINT_COUNT; ++i)
		{
			m_ownerWorld->removeConstraint(m_joints[i]);
			delete m_joints[i]; m_joints[i] = 0;
		}

		// Remove all bodies and shapes
		for ( i = 0; i < BODYPART_COUNT; ++i)
		{
			m_ownerWorld->removeRigidBody(m_bodies[i]);
			
			delete m_bodies[i]->getMotionState();

			delete m_bodies[i]; m_bodies[i] = 0;
			delete m_shapes[i]; m_shapes[i] = 0;
		}
	}

void BenchmarkDemo::createTest3()
{
	setCameraDistance(btScalar(50.));

	int size = 16;

	float sizeX = 1.f;
	float sizeY = 1.f;

	//int rc=0;

	btScalar scale(3.5);
	btVector3 pos(0.0f, sizeY, 0.0f);
	while(size) {
		float offset = -size * (sizeX * 6.0f) * 0.5f;
		for(int i=0;i<size;i++) {
			pos[0] = offset + (float)i * (sizeX * 6.0f);

				RagDoll* ragDoll = new RagDoll (m_dynamicsWorld,pos,scale);
				m_ragdolls.push_back(ragDoll);
		}

		offset += sizeX;
		pos[1] += (sizeY * 7.0f);
		pos[2] -= sizeX * 2.0f;
		size--;
	}

}
void BenchmarkDemo::createTest4()
{
	setCameraDistance(btScalar(50.));

	int size = 8;
	const float cubeSize = 1.5f;
	float spacing = cubeSize;
	btVector3 pos(0.0f, cubeSize * 2, 0.0f);
	float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;

	btConvexHullShape* convexHullShape = new btConvexHullShape();

	btScalar scaling(1);

	convexHullShape->setLocalScaling(btVector3(scaling,scaling,scaling));

	for (int i=0;i<TaruVtxCount;i++)
	{
		btVector3 vtx(TaruVtx[i*3],TaruVtx[i*3+1],TaruVtx[i*3+2]);
		convexHullShape->addPoint(vtx*btScalar(1./scaling));
	}

	//this will enable polyhedral contact clipping, better quality, slightly slower
	//convexHullShape->initializePolyhedralFeatures();

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	convexHullShape->calculateLocalInertia(mass,localInertia);

	for(int k=0;k<15;k++) {
		for(int j=0;j<size;j++) {
			pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
			for(int i=0;i<size;i++) {
				pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
				trans.setOrigin(pos);
				createRigidBody(mass,trans,convexHullShape);		
			}
		}
		offset -= 0.05f * spacing * (size-1);
		spacing *= 1.01f;
		pos[1] += (cubeSize * 2.0f + spacing);
	}
}


///////////////////////////////////////////////////////////////////////////////
// LargeMesh

// int LandscapeVtxCount[] = {
// 	Landscape01VtxCount,
// 	Landscape02VtxCount,
// 	Landscape03VtxCount,
// 	Landscape04VtxCount,
// 	Landscape05VtxCount,
// 	Landscape06VtxCount,
// 	Landscape07VtxCount,
// 	Landscape08VtxCount,
// };

// int LandscapeIdxCount[] = {
// 	Landscape01IdxCount,
// 	Landscape02IdxCount,
// 	Landscape03IdxCount,
// 	Landscape04IdxCount,
// 	Landscape05IdxCount,
// 	Landscape06IdxCount,
// 	Landscape07IdxCount,
// 	Landscape08IdxCount,
// };

// btScalar *LandscapeVtx[] = {
// 	Landscape01Vtx,
// 	Landscape02Vtx,
// 	Landscape03Vtx,
// 	Landscape04Vtx,
// 	Landscape05Vtx,
// 	Landscape06Vtx,
// 	Landscape07Vtx,
// 	Landscape08Vtx,
// };

// btScalar *LandscapeNml[] = {
// 	Landscape01Nml,
// 	Landscape02Nml,
// 	Landscape03Nml,
// 	Landscape04Nml,
// 	Landscape05Nml,
// 	Landscape06Nml,
// 	Landscape07Nml,
// 	Landscape08Nml,
// };

// btScalar* LandscapeTex[] = {
// 	Landscape01Tex,
// 	Landscape02Tex,
// 	Landscape03Tex,
// 	Landscape04Tex,
// 	Landscape05Tex,
// 	Landscape06Tex,
// 	Landscape07Tex,
// 	Landscape08Tex,
// };

// unsigned short  *LandscapeIdx[] = {
// 	Landscape01Idx,
// 	Landscape02Idx,
// 	Landscape03Idx,
// 	Landscape04Idx,
// 	Landscape05Idx,
// 	Landscape06Idx,
// 	Landscape07Idx,
// 	Landscape08Idx,
// };

// void BenchmarkDemo::createLargeMeshBody()
// {
// 	btTransform trans;
// 	trans.setIdentity();

// 	for(int i=0;i<8;i++) {

// 		btTriangleIndexVertexArray* meshInterface = new btTriangleIndexVertexArray();
// 		btIndexedMesh part;

// 		part.m_vertexBase = (const unsigned char*)LandscapeVtx[i];
// 		part.m_vertexStride = sizeof(btScalar) * 3;
// 		part.m_numVertices = LandscapeVtxCount[i];
// 		part.m_triangleIndexBase = (const unsigned char*)LandscapeIdx[i];
// 		part.m_triangleIndexStride = sizeof( short) * 3;
// 		part.m_numTriangles = LandscapeIdxCount[i]/3;
// 		part.m_indexType = PHY_SHORT;

// 		meshInterface->addIndexedMesh(part,PHY_SHORT);

// 		bool	useQuantizedAabbCompression = true;
// 		btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(meshInterface,useQuantizedAabbCompression);
// 		btVector3 localInertia(0,0,0);
// 		trans.setOrigin(btVector3(0,-25,0));

// 		btRigidBody* body = createRigidBody(0,trans,trimeshShape);
// 		body->setFriction (btScalar(0.9));
		
// 	}
	
// }


void BenchmarkDemo::createTest5()
{
	//setCameraDistance(btScalar(250.));
	btVector3 boxSize(1.5f,1.5f,1.5f);
	float boxMass = 1.0f;
	float sphereRadius = 1.5f;
	float sphereMass = 1.0f;
	float capsuleHalf = 2.0f;
	float capsuleRadius = 1.0f;
	float capsuleMass = 1.0f;

	btRigidBody* tempBody = NULL;

	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 0.0f, 20.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
		int numBodies = 0;

		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[1] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,0,300) + btVector3(5.0f,5.0f,1.0f)*pos + initOffset;
					int idx = rand() % 9;
					btTransform trans;
					trans.setIdentity();
					trans.setOrigin(bpos);

					switch(idx) {
						case 0:case 1:case 2:
						{
							float r = 0.5f * (idx+1);
							btBoxShape* boxShape = new btBoxShape(boxSize*r);
							tempBody = createRigidBody(boxMass*r,trans,boxShape);
						}
						break;

						case 3:case 4:case 5:
						{
							float r = 0.5f * (idx-3+1);
							btSphereShape* sphereShape = new btSphereShape(sphereRadius*r);
							tempBody = createRigidBody(sphereMass*r,trans,sphereShape);
						}
						break;

						case 6:case 7:case 8:
						{
							float r = 0.5f * (idx-6+1);
							btCapsuleShape* capsuleShape = new btCapsuleShape(capsuleRadius*r,capsuleHalf*r);
							tempBody = createRigidBody(capsuleMass*r,trans,capsuleShape);
						}
						break;
					}
					
					tempBody->bodyUID = -1;
					tempBody->limbUID = -1;
					
					numBodies++;
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}

	//createLargeMeshBody();
}
void BenchmarkDemo::createTest6()
{
	setCameraDistance(btScalar(250.));

	btVector3 boxSize(1.5f,1.5f,1.5f);

	btConvexHullShape* convexHullShape = new btConvexHullShape();

	for (int i=0;i<TaruVtxCount;i++)
	{
		btVector3 vtx(TaruVtx[i*3],TaruVtx[i*3+1],TaruVtx[i*3+2]);
		convexHullShape->addPoint(vtx);
	}

	btTransform trans;
	trans.setIdentity();

	float mass = 1.f;
	btVector3 localInertia(0,0,0);
	convexHullShape->calculateLocalInertia(mass,localInertia);


	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 20.0f, 0.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
	
		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,25,0) + btVector3(5.0f,1.0f,5.0f)*pos;
					trans.setOrigin(bpos);
					
					createRigidBody(mass,trans,convexHullShape);
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}


	//createLargeMeshBody();
}


void BenchmarkDemo::initRays()
{
	raycastBar = btRaycastBar2 (2500.0, 0,50.0,m_guiHelper);
}



void BenchmarkDemo::castRays()
{
	raycastBar.cast (m_dynamicsWorld);
}

void BenchmarkDemo::createTest7()
{
	
	createTest6();
	setCameraDistance(btScalar(150.));
	initRays();
}


void BenchmarkDemo::createTest8()
{
	setCameraDistance(btScalar(250.));
	btVector3 boxSize(1.5f,1.5f,1.5f);
	float boxMass = 1.0f;
	float sphereRadius = 1.5f;
	float sphereMass = 1.0f;
	float capsuleHalf = 2.0f;
	float capsuleRadius = 1.0f;
	float capsuleMass = 1.0f;

	{
		int size = 10;
		int height = 10;

		const float cubeSize = boxSize[0];
		float spacing = 2.0f;
		btVector3 pos(0.0f, 20.0f, 0.0f);
		float offset = -size * (cubeSize * 2.0f + spacing) * 0.5f;
		
		int numBodies = 0;

		for(int k=0;k<height;k++) {
			for(int j=0;j<size;j++) {
				pos[2] = offset + (float)j * (cubeSize * 2.0f + spacing);
				for(int i=0;i<size;i++) {
					pos[0] = offset + (float)i * (cubeSize * 2.0f + spacing);
					btVector3 bpos = btVector3(0,25,0) + btVector3(5.0f,1.0f,5.0f)*pos;
					int idx = rand() % 9;
					btTransform trans;
					trans.setIdentity();
					trans.setOrigin(bpos);

					switch(idx) {
						case 0:case 1:case 2:
						{
							float r = 0.5f * (idx+1);
							btBoxShape* boxShape = new btBoxShape(boxSize*r);
							createRigidBody(boxMass*r,trans,boxShape);
						}
						break;

						case 3:case 4:case 5:
						{
							float r = 0.5f * (idx-3+1);
							btSphereShape* sphereShape = new btSphereShape(sphereRadius*r);
							createRigidBody(sphereMass*r,trans,sphereShape);
						}
						break;

						case 6:case 7:case 8:
						{
							float r = 0.5f * (idx-6+1);
							btCapsuleShape* capsuleShape = new btCapsuleShape(capsuleRadius*r,capsuleHalf*r);
							createRigidBody(capsuleMass*r,trans,capsuleShape);
						}
						break;
					}

					numBodies++;
				}
			}
			offset -= 0.05f * spacing * (size-1);
			spacing *= 1.1f;
			pos[1] += (cubeSize * 2.0f + spacing);
		}
	}

	//createLargeMeshBody();
}


void BenchmarkDemo::exitPhysics()
{
	int i;

	for (i=0;i<m_ragdolls.size();i++)
	{
		RagDoll* doll = m_ragdolls[i];
		delete doll;
	}
	m_ragdolls.clear();

	CommonRigidBodyBase::exitPhysics();

	
}



void initNetMasks() {
	btVector3 nmDirs[8];
	
	int i;
	int j;
	int k;
	
	int ii;
	int jj;
	int kk;
	
	int tot;
	
	for (i = 0; i < 256; i++) {
		NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
	}
	
	
	for (k = -1; k <= 1; k += 2) {
		for (j = -1; j <= 1; j += 2) {
			for (i = -1; i <= 1; i += 2) {
				ii = (i + 1)/2;
				jj = (j + 1)/2;
				kk = (k + 1)/2;
				
				nmDirs[ii+jj*2+kk*4] = btVector3(i,j,k);
				
			}	
		}
	}
	
	for (i = 0; i < 8; i++) {
		nmDirs[i].normalize();
	}
	
	
	for (i = 0; i < 256; i++) {
		tot = 0;
		for (j = 0; j < 8; j++) {
			if ((i&(1<<j)) > 0) {
				NET_MASKS[i] += nmDirs[j];
				tot++;
			}
		}
		
		if (
			(
				abs(NET_MASKS[i].getX()) +
				abs(NET_MASKS[i].getY()) +
				abs(NET_MASKS[i].getZ())		
			) == 0.0
		) {
			NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
		}
		else {
			NET_MASKS[i].normalize();
			
			 // sqrt(0.5)/2 //0.35355339059f
			
			switch (tot) {
				
				case 1:
				case 7:
					NET_MASKS[i] = NET_MASKS[i] * (1.0f-0.43301270189f);//1.73205080757f / 4.0f; // sqrt(3)/4
				break;
				case 2:
				case 6:
					NET_MASKS[i] = NET_MASKS[i] * 1.41421356237f / 4.0f; // sqrt(2)/4
				break;
				// case 3:
				// 	NET_MASKS[i] = NET_MASKS[i] * -(1.0f-0.43301270189f);
				// break;
				default:
					NET_MASKS[i] = btVector3(0.0f,0.0f,0.0f);
				break;
				
			}
			
			if (tot > 4) {
				NET_MASKS[i] = -(NET_MASKS[i]);
			}
			
		}
		
		
		
		
		
	}
	
	
}

bool replaceStr(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// struct CommonExampleInterface*    BenchmarkCreateFunc(struct CommonExampleOptions& options)
// {
// 	return new BenchmarkDemo(options.m_guiHelper,options.m_option);
// }







