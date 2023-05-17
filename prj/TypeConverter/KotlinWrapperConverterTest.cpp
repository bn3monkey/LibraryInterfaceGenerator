#include "../../src/library/KotlinWrapperConverter/KotlinWrapperConverter.hpp"
#include <gtest/gtest.h>


#include <jni.h>
class JavaApplication
{
public:
	JavaApplication() {
		JavaVMInitArgs vm_args;
		JavaVMOption options[1];
		options[0].optionString = "-Djava.class.path=/usr/lib/java";
		vm_args.version = JNI_VERSION_10;
		vm_args.nOptions = 1;
		vm_args.options = options;
		vm_args.ignoreUnrecognized = JNI_TRUE;
		
		JNI_CreateJavaVM(&_jvm, (void **) &_env, &vm_args);
	}

	virtual ~JavaApplication() {
		_jvm->DestroyJavaVM();
	}

	const JavaVM* jvm() {
		return _jvm;
	}
	const JNIEnv* env() {
		return _env;
	}

private:
	JavaVM* _jvm;
	JNIEnv* _env;
};


TEST(KotlinWrapperConverterTest, Primitive)
{
	{
		JavaApplication application;
		printf("5\n");
	}
}