package net.poksion.end2;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

@SuppressLint("NewApi")
public final class LauncherActivity extends Activity {

	private GLSurfaceView mGLSurfaceView;

	@Override
	public void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mGLSurfaceView = new GLSurfaceView(this);
		mGLSurfaceView.setEGLContextClientVersion(1);
		//mGLSurfaceView.setEGLContextClientVersion(2);
		mGLSurfaceView.setRenderer(new GLSurfaceView.Renderer(){

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                JniBridge.nativeOnSurfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                JniBridge.nativeOnSurfaceChanged(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                JniBridge.nativeOnDrawFrame();
            }
		    
		});
		setContentView(mGLSurfaceView);
	}

	@Override
	public void onResume() {
		super.onResume();
		mGLSurfaceView.onResume();
	}

	@Override
	public void onPause() {
		super.onPause();
		mGLSurfaceView.onPause();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e) {

	    float x = e.getX();
	    float y = e.getY();

	    switch (e.getAction()) {
	        case MotionEvent.ACTION_DOWN : {
	            JniBridge.nativeOnMotionEventDown(0, x, y);
	        }
	        break;
	        case MotionEvent.ACTION_UP :
	        case MotionEvent.ACTION_CANCEL : {
	            JniBridge.nativeOnMotionEventUp(0, x, y);
	        }
	        break;
	        case MotionEvent.ACTION_MOVE : {
	            JniBridge.nativeOnMotionEventMove(0, x, y);
	        }
	        break;
	    }
	    return true;
	}

}