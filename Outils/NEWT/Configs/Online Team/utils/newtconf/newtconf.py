import sys

header = """<?xml version="1.0" encoding="UTF-8"?>
<Emulation xmlns="http://research.microsoft.com/asia">
"""

# name, filter, virtual_link
virtual_chan = """	<VirtualChannel name="%s" DispatchType="packet">
%s
%s
	</VirtualChannel>
"""

template_physaddr = """				<PhysicalAddress>%s</PhysicalAddress>"""

# name, template_physaddr, adapter, ip
template_filter = """		<FilterList>
			<Filter name="%s.filter" not="0">
%s
				<IpVersion>IPv4</IpVersion>
				<Local>
					<IpAddress>0.0.0.0</IpAddress>
					<IpMask>0.0.0.0</IpMask>
					<PortBegin>0</PortBegin>
					<PortEnd>65535</PortEnd>
				</Local>
				<Remote>
					<IpAddress>%s</IpAddress>
					<IpMask>255.255.255.255</IpMask>
					<PortBegin>0</PortBegin>
					<PortEnd>65535</PortEnd>
				</Remote>
			</Filter>
		</FilterList>"""


# name
# up bwlimit, up minlat, up maxlat, up errorrate, up periodicloss
# down bwlimit, down minlat, down maxlat, down errorrate, down periodicloss
template_vlink = """		<VirtualLink name="LINK %s.filter" instances="1">
			<LinkRule dir="upstream">
%s			</LinkRule>
			<LinkRule dir="downstream">
%s			</LinkRule>
		</VirtualLink>"""

template_bwlimit = """				<Bandwidth>
					<Speed unit="kbps">%d</Speed>
%s
				</Bandwidth>"""

# queue size, mode(packet), type(DropTail)
template_queue_mgmt = """					<QueueManagement>
						<NormalQueue>
							<Size>%d</Size>
							<QueueMode>%s</QueueMode>
							<DropType>%s</DropType>
						</NormalQueue>
					</QueueManagement>"""

template_latency_uniform = """				<Latency>
					<Uniform>
						<Min unit="msec">%d</Min>
						<Max unit="msec">%d</Max>
					</Uniform>
				</Latency>"""

template_latency_fixed = """				<Latency>
					<Fixed>
						<Time unit="msec">%d</Time>
					</Fixed>
				</Latency>"""
				
template_error = """				<Error>
					<Random>
						<ErrorUnit>packet</ErrorUnit>
						<Rate>%d</Rate>
					</Random>
				</Error>"""

template_loss = """				<Loss>
					<Periodic>
						<PerPackets>%d</PerPackets>
					</Periodic>
				</Loss>"""



footer = """
</Emulation>"""

class BwLimit(object):

	def __init__(self, bw=-1, qsize=0, qmode='packet', qtype='DropTail'):
		self.bw = bw
		self.bwqueue = {}
		if qsize:
			self.bwqueue['qsize'] = qsize
			self.bwqueue['qmode'] = qmode
			self.bwqueue['qtype'] = qtype


class Filter(object):

	def __init__(self):
		self.bwlimit = BwLimit()
		self.minlatency = 0
		self.maxlatency = 0
		self.errorrate = 0
		self.periodicloss = 0


class TestConfig(object):

	def __init__(self):
		self.name = 'default'
		self.adapters = {}
		self.upfilter = Filter()
		self.downfilter = Filter()
		self.consoles = {}

class ConfigLoader(object):
			
	def __init__(self, testconfig):
		self.symbols = {}
		self.prefix = "conf_"
		self.testconf = testconfig
		self.current_stream_filter = None

		# build a dictionnary with prefixed functions
		for name in dir(self.__class__):
			prefix_pos = len(self.prefix)
			if name[:prefix_pos] == self.prefix:
				self.symbols[name[prefix_pos:]] = getattr(self, name)

	# execute an arbitrary string of code
	def exec_code(self, code):
		if code != "":
			exec(code, self.symbols, self.symbols)
		else:
			print "ERROR : no code to execute!"

	# execute a file
	def exec_file(self, filename):
		if filename != "":
			execfile(filename, self.symbols, self.symbols)
		else:
			print "ERROR : no filename provided!"
		
	def conf_name(self, name):
		self.testconf.name = name

	def conf_adapters(self, adapters):
		self.testconf.adapters = adapters

	def conf_upstream(self):
		self.current_stream_filter = self.testconf.upfilter

	def conf_downstream(self):
		self.current_stream_filter = self.testconf.downfilter

	def conf_bw_filter(self, bw=-1, qsize=0, qmode='packet', qtype='DropTail'):
		self.current_stream_filter.bwlimit = BwLimit(bw, qsize, qmode, qtype)

	def conf_latency_filter(self,  minlatency=0, maxlatency=0):
		self.current_stream_filter.minlatency = minlatency
		self.current_stream_filter.maxlatency = maxlatency

	def conf_error_filter(self, rate):
		self.current_stream_filter.errorrate = rate

	def conf_loss_filter(self, loss):
		self.current_stream_filter.periodicloss = loss

	def conf_add_console(self, name, ip, adapter=-1):
		self.testconf.consoles[name] = [ip, adapter]


def generate_xml_stream_rule(filter):
	# upstream
	xml_result = ''
	if filter.bwlimit.bw > -1:
		xml_bw_queue = ''
		if len(filter.bwlimit.bwqueue):
			queueParam = filter.bwlimit.bwqueue
			xml_bw_queue = template_queue_mgmt % (queueParam['qsize'], queueParam['qmode'], queueParam['qtype'])
		xml_result += template_bwlimit % (filter.bwlimit.bw, xml_bw_queue)
		xml_result += '\n'

	if filter.minlatency > 0 or filter.maxlatency > 0:
		if filter.maxlatency == 0:
			xml_result += template_latency_fixed % filter.minlatency
		else:
			xml_result += template_latency_uniform % (filter.minlatency, filter.maxlatency)
		xml_result += '\n'
	
	if filter.errorrate > 0:
		xml_result += template_error % filter.errorrate
		xml_result += '\n'
	
	if filter.periodicloss > 0:
		xml_result += template_loss % filter.periodicloss
		xml_result += '\n'

	return xml_result

def generate_xml(name, config):
	f = file(name, "w")

	f.write(header)

	for c in config.consoles.iterkeys():
		ip = config.consoles[c][0]
		adapterid = config.consoles[c][1]
		xml_physaddr = ""
		if adapterid > -1:
			xml_physaddr = template_physaddr % config.adapters[adapterid]

		xml_filter = template_filter % (c, xml_physaddr, ip)

		xml_uprule = generate_xml_stream_rule(config.upfilter)
		xml_downrule = generate_xml_stream_rule(config.downfilter)

		# link with up & down stream conditions
		xml_vlink = template_vlink % (c, xml_uprule, xml_downrule)

		# final channel
		txt = virtual_chan % (c, xml_filter, xml_vlink)
		f.write(txt)

	f.write(footer)
	f.close();


## ========================================================
## MAIN
## ========================================================
generator_config = TestConfig()
config_loader = ConfigLoader(generator_config)

configfile = "default"
if len(sys.argv) > 1:
	configfile = sys.argv[1]

print "Parsing %s" % configfile

config_loader.exec_file(configfile)

print "Generating NEWT file:  %s.xml" % generator_config.name
generate_xml("%s.xml" % generator_config.name, generator_config)

print "DONE."
