Summary:         Fits 3 data handler for the OPeNDAP Data server
Name:            fits_handler
Version:         1.0.3
Release:         1
License:         LGPL
Group:           System Environment/Daemons 
Source0:         http://www.opendap.org/pub/source/%{name}-%{version}.tar.gz
URL:             http://www.opendap.org/

BuildRoot:       %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:   libdap-devel >= 3.8.0 netcdf-devel
BuildRequires:   bes-devel >= 3.6.0

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
%{_bindir}/bes-fits-data.sh
%{_libdir}/libfits_handler.so.*
%{_libdir}/bes/libfits_module.so
%{_datadir}/hyrax/
%doc COPYING COPYRIGHT NEWS README

%changelog
* Mon Mar 03 2008 Patrick West <pwest@ucar.edu> 1.0.3-1
- Release 1.0.3

* Wed Jan 10 2007 Patrick West <pwest@ucar.edu> 1.0.1-1
- initial release

