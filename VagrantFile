Vagrant.configure("2") do |config|
    config.vm.box = "ubuntu/trusty64"
    config.vm.provider "virtualbox" do |vb|    
      vb.cpus = 1
      vb.memory = 2048
    end
    config.vm.synced_folder ".", "/ft_ping", type: "virtualbox"  
end