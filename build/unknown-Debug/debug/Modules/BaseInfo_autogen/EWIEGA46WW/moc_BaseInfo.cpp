/****************************************************************************
** Meta object code from reading C++ file 'BaseInfo.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../BaseInfo.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BaseInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSBaseInfoModuleENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSBaseInfoModuleENDCLASS = QtMocHelpers::stringData(
    "BaseInfoModule"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSBaseInfoModuleENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject BaseInfoModule::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSBaseInfoModuleENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSBaseInfoModuleENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSBaseInfoModuleENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BaseInfoModule, std::true_type>
    >,
    nullptr
} };

void BaseInfoModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *BaseInfoModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseInfoModule::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSBaseInfoModuleENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "ModuleTemplate"))
        return static_cast< ModuleTemplate*>(this);
    if (!strcmp(_clname, "org.example.Monogatari.ModuleTemplate"))
        return static_cast< ModuleTemplate*>(this);
    return QObject::qt_metacast(_clname);
}

int BaseInfoModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_BaseInfoModule[] = {
    0xbf, 
    // "IID"
    0x02,  0x78,  0x25,  'o',  'r',  'g',  '.',  'e', 
    'x',  'a',  'm',  'p',  'l',  'e',  '.',  'M', 
    'o',  'n',  'o',  'g',  'a',  't',  'a',  'r', 
    'i',  '.',  'M',  'o',  'd',  'u',  'l',  'e', 
    'T',  'e',  'm',  'p',  'l',  'a',  't',  'e', 
    // "className"
    0x03,  0x6e,  'B',  'a',  's',  'e',  'I',  'n', 
    'f',  'o',  'M',  'o',  'd',  'u',  'l',  'e', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(BaseInfoModule, BaseInfoModule, qt_pluginMetaDataV2_BaseInfoModule)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_BaseInfoModule[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x25,  'o',  'r',  'g',  '.',  'e', 
    'x',  'a',  'm',  'p',  'l',  'e',  '.',  'M', 
    'o',  'n',  'o',  'g',  'a',  't',  'a',  'r', 
    'i',  '.',  'M',  'o',  'd',  'u',  'l',  'e', 
    'T',  'e',  'm',  'p',  'l',  'a',  't',  'e', 
    // "className"
    0x03,  0x6e,  'B',  'a',  's',  'e',  'I',  'n', 
    'f',  'o',  'M',  'o',  'd',  'u',  'l',  'e', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(BaseInfoModule, BaseInfoModule)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP
