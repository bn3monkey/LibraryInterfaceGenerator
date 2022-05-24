# Library Interface Generator

- [Library Interface Generator](#library-interface-generator)
  - [Introduction (소개)](#introduction-소개)
  - [Required JSON format (요구되는 JSON 포맷)](#required-json-format-요구되는-json-포맷)
    - [Package (패키지)](#package-패키지)
    - [Module (모듈)](#module-모듈)
    - [Interface (인터페이스)](#interface-인터페이스)
    - [Class (클래스)](#class-클래스)
    - [Type (타입)](#type-타입)
    - [Property (프로퍼티)](#property-프로퍼티)
    - [Method (메소드)](#method-메소드)
    - [Parameter (파라미터)](#parameter-파라미터)

This project converts the json that specifies the software module in the library to the API document, C/C++ header of the library, and library interface for each language.

이 프로젝트는 라이브러리에 있는 소프트웨어 모듈을 명세하는 json을 언어별 API 문서, 라이브러리의 C/C++ 헤더, 라이브러리 인터페이스로 변환합니다.

## Introduction (소개)

![software_library_hierarchy_1](image/software_library_hierarchy_1.png)

This software assumes that the library has the structure shown in the figure above.

A library has one top-level unit that contains all the functions of software. We define this as **package**.

A package is divided into several software units according to the logical classification of functions. We define this unit as **module**.

Modules can be subdivided into smaller units of modules.

Modules have **classes** and **interfaces** and **global method**.  
A class has **properties**, which are specifications for data, and **methods**, which are specifications for operations.  
Interfaces also have properties and methods.
 
An interface differs from a class in that an interface is a guideline for the functionality that a class from which it inherits must implement. Therefore, the functionality required by the interface is not implemented inside the interface. The implementation of the feature is implemented within a class that inherits the interface.

**Method** holds **parameters**, a set of data required to implement a function.

The library exports the functions provided by the software units introduced above as multiple functions that can be used externally.

이 소프트웨어는 라이브러리를 위 그림과 같은 구조를 가지고 있다고 가정하고 있습니다.

라이브러리는 소프트웨어가 가지고 있는 기능들을 모두 가지고 있는 하나의 최상위 단위가 있습니다. 이를 **패키지**라고 정의 합니다.

패키지는 기능의 논리적 분류에 따라 여러 가지의 소프트웨어 단위로 분화됩니다. 이 단위를 **모듈**로 정의합니다.

모듈은 더 작은 단위의 모듈로 분화될 수 있습니다.

모듈은 **클래스**와 **인터페이스**, **전역 메소드**를 보유합니다.  
클래스는 데이터에 대한 명세인 **프로퍼티**와 작업에 대한 명세인 **메소드**를 보유하고 있습니다.  
인터페이스 역시 프로퍼티와 메소드를 보유하고 있습니다.
  
인터페이스가 클래스와 다른 점은 인터페이스는 자신을 상속한 클래스가 구현해야 할 기능에 대한 가이드라인인 것입니다. 따라서, 인터페이스 내부에서 인터페이스가 요구되는 기능이 구현되지 않습니다. 기능의 구현은 인터페이스를 상속한 클래스 내에서 구현됩니다.

**메소드**는 기능을 구현하기 위한 필요한 데이터의 집합인 **파라미터**들을 보유하고 있습니다.

라이브러리는 위에서 소개한 소프트웨어 단위들이 제공하는 기능들을 외부에서 사용할 수 있는 여러 개의 함수로 내보냅니다. 

![software_library_hierarchy_2](image/software_library_hierarchy_2.png)

This project assumes that the user of the library will want to use the internal structure of the C/C++ library in the same form in various programming languages ​​and frameworks.
 
However, software libraries generally provide all functionality as a set of functions.

In order to reproduce the internal structure of the library in an external framework, the following process is required.

1. Provides a set of functions to use the functions of the library.
2. Attach a wrapper function to make a set of functions available to any framework.
3. Reproduce the internal structure in the framework you are using using wrapper functions.

이 프로젝트는 라이브러리의 사용자가 C/C++ 라이브러리가 가지고 있는 내부 구조를 다양한 프로그래밍 언어 및 프레임워크에서도 동일한 형태로 사용하기 원할 것을 가정하고 있습니다.

하지만, 소프트웨어 라이브러리는 일반적으로 모든 기능이 일련의 함수들로만 제공합니다.

외부 프레임워크에서 라이브러리의 내부 구조를 재현하기 위해서는 아래의 과정을 필요로 합니다.

1. 라이브러리의 기능을 사용할 수 있도록 일련의 함수를 제공합니다.
2. 일련의 함수를 어떤 프레임워크에서 사용할 수 있도록 래퍼 함수를 붙입니다.
3. 래퍼 함수들을 이용하여 사용하는 프레임워크에서 내부 구조를 재현합니다.

![software_library_hierarchy_3](image/software_library_hierarchy_3.png)

This project converts the module structure modeled in JSON into the output below.

1. API Documentation (Markdown)
2. Directories and C/C++ header files for each package, module, interface, and class
3. C/C++ headers for functions exported outside the library
4. Library wrapper code for target language and framework
5. A directory for each package, module, interface, class and code for that language/framework.

이 프로젝트는 JSON으로 모델링한 모듈 구조를 아래의 결과물로 변환합니다.

1. API 문서(Markdown)
2. 각 패키지, 모듈, 인터페이스, 클래스에 대한 디렉토리와 C/C++ 헤더 파일
3. 라이브러리 외부에로 내보내진 함수에 대한 C/C++ 헤더
4. 타겟 언어 및 프레임워크에 대한 라이브러리 래퍼 코드
5. 각 패키지, 모듈, 인터페이스, 클래스에 대한 디렉토리와 해당 언어/프레임워크에 대한 코드

## Required JSON format (요구되는 JSON 포맷)

```json
{
    "order" : "package",
    "name" : "package A",
    "author" : "...",
    "description" : "...",
    "childs" : [
        {
            "order" : "module",
            "name" : "module A",
            "description" : "...",
            "childs" : [
                {
                    "order" : "module",
                    "name" : "module AA",
                    "description" : "...",
                    "childs" : [

                    ]
                }
            ]
        },
        {
            "order" : "module",
            "name" : "module B",
            "description" : "...",
            "childs" : [
                {
                    "order" : "interface",
                    "name" : "interface A",
                    "description" : "...",
                    "childs" : [
                        {
                            "order" : "property",
                            "type" : "int32",
                            "name" : "property A",
                            "description" : "..."
                        },
                        {
                            "order" : "method",
                            "type" : "int64",
                            "name" : "method A",
                            "description" : "...",
                            "parameters" : [
                                {
                                    "type" : "string",
                                    "name" : "param A",
                                    "description" : "..."
                                },
                                {
                                    "type" : "bool",
                                    "name" : "param B",
                                    "description" : "..."
                                }
                            ]
                        }
                    ]
                },
                {
                    "order" : "class",
                    "name" : "class A",
                    "base" : ["module B/interface A"],
                    "description" : "...",
                    "childs" : [
                        {
                            "order" : "property",
                            "type" : "int8",
                            "name" : "property C",
                            "description" : "..."
                        },
                        {
                            "order" : "method",
                            "type" : "int16",
                            "name" : "method D",
                            "description" : "...",
                            "parameters" : [
                                {
                                    "type" : "array<int32>",
                                    "name" : "param A",
                                    "description" : "..."
                                },
                                {
                                    "type" : "array<bool>",
                                    "name" : "param B",
                                    "description" : "..."
                                }
                            ]
                        }
                    ]
                },
                {
                    "order" : "method",
                    "type" : "",
                    "name" : "method D",
                    "description" : "...",
                    "parameters" : [
                        {
                            "type" : "double",
                            "name" : "param A",
                            "description" : "..."
                        },
                        {
                            "type" : "float",
                            "name" : "param B",
                            "description" : "..."
                        },
                        {
                            "type" : "array<module B/interface A>",
                            "name" : "param C",
                            "description" : "..."
                        },
                        {
                            "type" : "module B/class A",
                            "name" : "param D",
                            "description" : "..."
                        }
                    ]
                }
            ]
        }
    ]
}
```

Above is an overview of the JSON format your project will need to write in order to get the output it needs. 
Take a look at the JSON object for each order below.

위는 필요한 출력을 얻기 위해 프로젝트에서 작성해야 하는 JSON 형식의 개요입니다.  
아래에서 각 분류에 대한 JSON 객체를 살펴보세요.

### Package (패키지)

```json

```

### Module (모듈)

```json

```

### Interface (인터페이스)

```json

```

### Class (클래스)

```json

```

### Type (타입)

```json

```

### Property (프로퍼티)

```json

```

### Method (메소드)

```json

```

### Parameter (파라미터)

```json

```
