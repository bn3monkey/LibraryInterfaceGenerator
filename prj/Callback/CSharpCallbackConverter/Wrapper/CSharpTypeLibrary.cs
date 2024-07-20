using System.Runtime.InteropServices;

namespace Wrapper
{
    public class CSharpTypeLibrary
    {

        [DllImport("Library.dll", EntryPoint = "CSharpTypeLibrary_initialize")]
        private static extern IntPtr CSharpTypeLibrary_initialize();

        [DllImport("Library.dll", EntryPoint = "CSharpTypeLibrary_release")]
        private static extern void CSharpTypeLibrary_release(IntPtr context);


        [DllImport("Library.dll", EntryPoint = "CSharpTypeLibrary_registerCallback")]
        private static extern void CSharpTypeLibrary_registerCallback(IntPtr context, IntPtr callback);

        [DllImport("Library.dll", EntryPoint = "CSharpTypeLibrary_unregisterCallback")]
        private static extern void CSharpTypeLibrary_unregisterCallback(IntPtr context);
        [DllImport("Library.dll", EntryPoint = "CSharpTypeLibrary_runCallback")]
        private static extern void CSharpTypeLibrary_runCallback(IntPtr context, int param1, float param2);


        public IntPtr Initialize()
        {
            return CSharpTypeLibrary_initialize();
        }
        public void Release(IntPtr context)
        {
            CSharpTypeLibrary_release(context);
        }



        private delegate void Callback(int param1, float param2);
        Callback? _callback = null;
        
        public void RegisterCallback(IntPtr context, Action<int, float> callback)
        {
             _callback = new Callback(callback);
            IntPtr callbackPtr = Marshal.GetFunctionPointerForDelegate(_callback);
            CSharpTypeLibrary_registerCallback(context, callbackPtr);
        }

        public void UnregisterCallback(IntPtr context)
        {
            CSharpTypeLibrary_unregisterCallback(context);
            _callback = null;
        }

        public void RunCallback(IntPtr context, int param1, float param2)
        {
            CSharpTypeLibrary_runCallback(context, param1, param2);
        }
    }
}
