<?xml version="1.0" encoding="UTF-8"?>
<simconf version="2023090101">
  <simulation>
    <title>D-ConCReCT 100 nodes 10pct</title>
    <randomseed>12345</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>40000</logoutput>
    </events>
    <motetype>
      org.contikios.cooja.mspmote.SkyMoteType
      <description>Sky Mote Type #1</description>
      <source>[CONFIG_DIR]/../dconcrect-server.c</source>
      <commands>$(MAKE) -j$(CPUS) dconcrect-server.sky TARGET=sky</commands>
      <firmware>[CONFIG_DIR]/../build/sky/dconcrect-server.sky</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyFlash</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyCoffeeFilesystem</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyTemperature</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="100.0" y="100.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>1</id>
        </interface_config>
      </mote>
    </motetype>
    <motetype>
      org.contikios.cooja.mspmote.SkyMoteType
      <description>Sky Mote Type #2</description>
      <source>[CONFIG_DIR]/../dconcrect-client.c</source>
      <commands>$(MAKE) -j$(CPUS) dconcrect-client.sky TARGET=sky</commands>
      <firmware>[CONFIG_DIR]/../build/sky/dconcrect-client.sky</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyFlash</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyCoffeeFilesystem</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyTemperature</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="9.2" y="5.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>2</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="33.3" y="8.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>3</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="48.7" y="6.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>4</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="70.7" y="6.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>5</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="86.2" y="9.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>6</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="110.6" y="6.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>7</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="130.5" y="8.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>8</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="154.6" y="5.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>9</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="174.8" y="9.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>10</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="190.0" y="6.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>11</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="12.2" y="26.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>12</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="28.4" y="25.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>13</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="48.4" y="34.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>14</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="74.8" y="32.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>15</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="85.0" y="34.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>16</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="113.7" y="32.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>17</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="126.8" y="26.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>18</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="149.1" y="33.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>19</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="170.8" y="32.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>20</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="187.3" y="30.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>21</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="12.1" y="53.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>22</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="33.1" y="47.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>23</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="53.7" y="47.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>24</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="73.0" y="50.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>25</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="86.9" y="50.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>26</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="110.2" y="54.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>27</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="125.4" y="46.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>28</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="154.8" y="53.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>29</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="166.5" y="47.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>30</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="190.4" y="46.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>31</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="8.2" y="65.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>32</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="32.1" y="65.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>33</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="54.9" y="74.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>34</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="72.9" y="72.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>35</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="88.7" y="71.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>36</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="112.6" y="69.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>37</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="125.6" y="71.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>38</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="148.3" y="71.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>39</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="169.2" y="74.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>40</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="187.0" y="72.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>41</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="12.0" y="90.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>42</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="25.8" y="86.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>43</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="54.7" y="87.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>44</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="66.6" y="87.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>45</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="90.4" y="90.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>46</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="105.0" y="85.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>47</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="130.0" y="86.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>48</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="151.1" y="93.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>49</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="166.0" y="90.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>50</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="189.1" y="92.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>51</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="14.5" y="109.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>52</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="27.4" y="113.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>53</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="45.3" y="108.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>54</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="68.9" y="106.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>55</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="91.8" y="110.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>56</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="107.5" y="109.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>57</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="131.9" y="108.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>58</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="145.7" y="109.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>59</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="167.0" y="111.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>60</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="187.0" y="106.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>61</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="13.9" y="128.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>62</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="27.9" y="130.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>63</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="51.9" y="131.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>64</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="73.0" y="127.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>65</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="89.3" y="128.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>66</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="110.2" y="130.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>67</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="131.3" y="125.7" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>68</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="148.7" y="132.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>69</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="165.4" y="132.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>70</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="191.9" y="129.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>71</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="9.5" y="147.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>72</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="32.4" y="149.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>73</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="47.7" y="145.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>74</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="68.5" y="154.2" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>75</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="91.8" y="151.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>76</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="114.3" y="145.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>77</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="131.0" y="146.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>78</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="149.0" y="146.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>79</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="168.5" y="154.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>80</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="192.3" y="153.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>81</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="5.8" y="168.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>82</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="32.8" y="165.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>83</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="46.9" y="165.8" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>84</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="73.2" y="167.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>85</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="89.2" y="165.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>86</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="106.1" y="170.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>87</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="132.9" y="168.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>88</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="148.2" y="165.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>89</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="166.0" y="166.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>90</id>
        </interface_config>
      </mote>
    </motetype>
    <motetype>
      org.contikios.cooja.mspmote.SkyMoteType
      <description>Sky Mote Type #3</description>
      <source>[CONFIG_DIR]/../dconcrect-attacker.c</source>
      <commands>$(MAKE) -j$(CPUS) dconcrect-attacker.sky TARGET=sky</commands>
      <firmware>[CONFIG_DIR]/../build/sky/dconcrect-attacker.sky</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyButton</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyFlash</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyCoffeeFilesystem</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspSerial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.SkyTemperature</moteinterface>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="47.1" y="112.6" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>91</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="74.6" y="186.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>92</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="61.1" y="164.1" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>93</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="26.5" y="105.0" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>94</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="108.9" y="128.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>95</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="83.8" y="104.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>96</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="150.0" y="37.3" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>97</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="105.7" y="115.9" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>98</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="150.7" y="39.5" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>99</id>
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="84.6" y="14.4" />
        </interface_config>
        <interface_config>
          org.contikios.cooja.mspmote.interfaces.MspMoteID
          <id>100</id>
        </interface_config>
      </mote>
    </motetype>
  </simulation>
</simconf>
