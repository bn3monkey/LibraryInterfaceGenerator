package com.example.wrapper

class Wrapper {

    /**
     * A native method that is implemented by the 'nativelibrary' native library,
     * which is packaged with this application.
     */


    external fun initalize() : Long
    external fun release(context : Long)

    external fun registerCallback(context : Long, callback : (Int, Float) -> Unit)
    external fun unregisterCallback(context : Long)
    external fun runCallback(context : Long, param1 : Int, param2 : Float)

    companion object {
        // Used to load the 'nativelibrary' library on application startup.
        init {
            System.loadLibrary("wrapper")
        }
    }
}