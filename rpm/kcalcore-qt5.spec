Name:       kcalcore-qt5
Summary:    Port of KDE kcalcore calendar library
Version:    4.10.2
Release:    1
Group:      System/Libraries
License:    LGPLv2
URL:        https://github.com/mer-packages/kcalcore
Source0:    %{name}-%{version}.tar.bz2
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(libical)
BuildRequires:  pkgconfig(uuid)
BuildRequires:  pkgconfig(timed-qt5)

%description
Port of KDE kcalcore calendar library

%package devel
Summary:    Development files for kcalcore
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
This package contains the files necessary to develop
applications using kcalcore

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5 
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libkcalcoren-qt5.so.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/kcalcoren-qt5/*
%{_libdir}/libkcalcoren-qt5.so
%{_libdir}/pkgconfig/*.pc
