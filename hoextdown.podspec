Pod::Spec.new do |s|
  s.name = 'hoextdown'
  s.version = '3.0.5.2'
  s.summary = 'Hoextdown is an extension to Hoedown - Standards compliant, fast, secure markdown processing library in C. A fork of sundown.'
  s.homepage = 'https://github.com/kjdev/hoextdown'
  s.license = 'MIT'
  s.author = { 'Natacha Porté' => '',
               'Vicent Martí' => '',
               'Xavier Mendez, Devin Torres and the Hoedown authors' => '',
               'kjdev' => '',
               'Ashok Gelal (podspec)' => 'ashokgelal@gmail.com' }
  s.source = { :git => 'https://github.com/kjdev/hoextdown', :tag => s.version }
  s.requires_arc = false

  s.default_subspec = 'standard'

  s.subspec 'standard' do |ss|
    ss.source_files = 'src/*.{c,h}'
  end
end
