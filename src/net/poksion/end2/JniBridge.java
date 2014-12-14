package net.poksion.end2;

public final class JniBridge {

	private static final String LIBRARY_NAME = "end2";

	static {
		System.loadLibrary(LIBRARY_NAME);
	}

	public static native void nativeOnSurfaceCreated();
	public static native void nativeOnSurfaceChanged(int width, int height);
	public static native void nativeOnDrawFrame();
	
	public static native void nativeOnMotionEventUp(int pointerIndex, float x, float y);
	public static native void nativeOnMotionEventDown(int pointerIndex, float x, float y);
	public static native void nativeOnMotionEventMove(int pointerIndex, float x, float y);
}