Summary:         Fits 3 data handler for the OPeNDAP Data server
Name:            fits_handler
Version:         1.0.1
Release:         1
License:         LGPL
Group:           System Environment/Daemons 
Source0:         ftp://ftp.unidata.ucar.edu/pub/opendap/source/%{name}-%{version}.tar.gz
URL:             http://www.opendap.org/

BuildRoot:       %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:   libdap-devel >= 3.7.4 netcdf-devel
BuildRequires:   bes-devel

%description
This is the fits data handler for our data server. It reads fits 3
files and returns DAP responses that are compatible with DAP2 and the
dap-server software.

%prep 
%setup -q

%build
%configure --disable-static --disable-dependency-tracking
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

rm -f $RPM_BUILD_ROOT%{_libdir}/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/*.so
rm -f $RPM_BUILD_ROOT%{_libdir}/bes/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_bindir}/dap_fits_handler
%{_libdir}/bes/*
%{_libdir}/lib*.so.*
%doc COPYING COPYRIGHT NEWS
%doc README

%changelog
* Wed Jan 10 2007 Patrick West <pwest@ucar.edu> 1.0.1-1
- initial release
