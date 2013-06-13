<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>FileOutput.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>FileOutput_8h</filename>
    <class kind="class">logging::FileOutput</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>Logger.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>Logger_8h</filename>
    <class kind="class">logging::detail::Logger</class>
    <class kind="struct">logging::log</class>
    <namespace>logging</namespace>
    <namespace>logging::detail</namespace>
    <member kind="define">
      <type>#define</type>
      <name>LOGGING_DEFINE_OUTPUT</name>
      <anchorfile>Logger_8h.shtml</anchorfile>
      <anchor>ac3224dde26c3698053aaa2d8ef86631e</anchor>
      <arglist>(BASE)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOGGING_DISABLE_LEVEL</name>
      <anchorfile>Logger_8h.shtml</anchorfile>
      <anchor>a5af4a04a0bbe25cb03fe2551a59f817b</anchor>
      <arglist>(NAME)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>LoggerLevel.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>LoggerLevel_8h</filename>
    <class kind="struct">logging::Level</class>
    <class kind="struct">logging::RunTimeLevel</class>
    <class kind="struct">logging::Error</class>
    <class kind="struct">logging::Warning</class>
    <class kind="struct">logging::Void</class>
    <class kind="struct">logging::Info</class>
    <class kind="struct">logging::Trace</class>
    <class kind="struct">logging::Debug</class>
    <namespace>logging</namespace>
    <member kind="define">
      <type>#define</type>
      <name>LOGGING_GENERATE_LEVEL</name>
      <anchorfile>LoggerLevel_8h.shtml</anchorfile>
      <anchor>a57dc551641c5ae999d6898ef74c3e1f7</anchor>
      <arglist>(LEVELNAME, LEVEL, DESC)</arglist>
    </member>
    <member kind="typedef">
      <type>Void</type>
      <name>Normal</name>
      <anchorfile>namespacelogging.shtml</anchorfile>
      <anchor>a4581e210ebea08835d723e1483765373</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>RunTimeLevel</type>
      <name>setLoggingLevel</name>
      <anchorfile>namespacelogging.shtml</anchorfile>
      <anchor>a1394cf978669d10a3c94440b02ecc91a</anchor>
      <arglist>(T level)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logging.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>logging_8h</filename>
  </compound>
  <compound kind="file">
    <name>loggingConfigAVR.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>loggingConfigAVR_8h</filename>
    <namespace>logging</namespace>
    <namespace>logging::config</namespace>
    <member kind="define">
      <type>#define</type>
      <name>UARTLOGDEVICE</name>
      <anchorfile>loggingConfigAVR_8h.shtml</anchorfile>
      <anchor>af8160402a2632bff23f34a8ba2874b49</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>::logging::OutputLevelSwitchDisabled&lt; ::logging::OutputStream&lt; COutDevice&lt; SecOut&lt; Uartnoint&lt; UARTLOGDEVICE&lt;&gt; &gt; &gt; &gt; &gt; &gt;</type>
      <name>UartLogType</name>
      <anchorfile>namespacelogging_1_1config.shtml</anchorfile>
      <anchor>ab6ded1e115b43cff3a3a8e56698530c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>::logging::config::UartLogType</type>
      <name>LoggingType</name>
      <anchorfile>namespacelogging.shtml</anchorfile>
      <anchor>a80a3189fa00e4626ac9135cfbb5ff7ae</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>loggingConfigGeneralPurposeOS.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>loggingConfigGeneralPurposeOS_8h</filename>
    <namespace>logging</namespace>
    <namespace>logging::config</namespace>
    <member kind="typedef">
      <type>::logging::OutputLevelSwitchDisabled&lt; ::logging::OutputStream&lt; ::logging::FileOutput &gt; &gt;</type>
      <name>FileLogType</name>
      <anchorfile>namespacelogging_1_1config.shtml</anchorfile>
      <anchor>adbd2749b423e797c8e9927fea6034b60</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>::logging::OutputLevelSwitchDisabled&lt; ::logging::OutputStream&lt; ::logging::StdOutput&lt;&gt; &gt; &gt;</type>
      <name>StdLogType</name>
      <anchorfile>namespacelogging_1_1config.shtml</anchorfile>
      <anchor>a4e243e6ba6dd3db49ba7303322358d91</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>::logging::OutputLevelSwitchDisabled&lt; ::logging::OutputStream&lt; ::logging::StdOutput&lt; ::std::cerr &gt; &gt; &gt;</type>
      <name>StdErrLogType</name>
      <anchorfile>namespacelogging_1_1config.shtml</anchorfile>
      <anchor>a8c6130447fc6264adac64e317bf791f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>::logging::OutputLevelRunTimeSwitch&lt; ::logging::OutputStream&lt; ::logging::StdOutput&lt;&gt; &gt; &gt;</type>
      <name>StdLogRunTimeSwitchType</name>
      <anchorfile>namespacelogging_1_1config.shtml</anchorfile>
      <anchor>a7044c50febe0a877d1c88a07fac575f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NullOutput.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>NullOutput_8h</filename>
    <class kind="class">logging::NullOutput</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>OutputLevelRunTimeSwitch.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>OutputLevelRunTimeSwitch_8h</filename>
    <class kind="class">logging::OutputLevelRunTimeSwitch</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>OutputLevelSwitchDisabled.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>OutputLevelSwitchDisabled_8h</filename>
    <class kind="class">logging::OutputLevelSwitchDisabled</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>OutputStream.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>OutputStream_8h</filename>
    <class kind="class">logging::OutputStream</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>ProgramMemoryString.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>ProgramMemoryString_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>__STRINGIFICATION__</name>
      <anchorfile>ProgramMemoryString_8h.shtml</anchorfile>
      <anchor>ac33cc84cea61a8282b89f94a102434c6</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__TOSTR__</name>
      <anchorfile>ProgramMemoryString_8h.shtml</anchorfile>
      <anchor>a0f6ecffc32338f1c07e97e731b3e9a83</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROGMEMSTRING</name>
      <anchorfile>ProgramMemoryString_8h.shtml</anchorfile>
      <anchor>a0f03391b952499eb443c75b49f30645f</anchor>
      <arglist>(S)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROGMEMSTRINGTYPE</name>
      <anchorfile>ProgramMemoryString_8h.shtml</anchorfile>
      <anchor>aefd4b606883ca0d109f905679c5b745a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ProgramMemoryStringImpl.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>ProgramMemoryStringImpl_8h</filename>
    <class kind="struct">logging::ProgramMemoryString</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="file">
    <name>singleton.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>singleton_8h</filename>
    <class kind="struct">logging::detail::singleton</class>
    <namespace>logging</namespace>
    <namespace>logging::detail</namespace>
  </compound>
  <compound kind="file">
    <name>StdOutput.h</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>StdOutput_8h</filename>
    <class kind="class">logging::StdOutput</class>
    <namespace>logging</namespace>
  </compound>
  <compound kind="namespace">
    <name>logging</name>
    <filename>namespacelogging.shtml</filename>
    <namespace>logging::config</namespace>
    <namespace>logging::detail</namespace>
    <class kind="class">logging::FileOutput</class>
    <class kind="struct">logging::log</class>
    <class kind="struct">logging::Level</class>
    <class kind="struct">logging::RunTimeLevel</class>
    <class kind="struct">logging::Error</class>
    <class kind="struct">logging::Warning</class>
    <class kind="struct">logging::Void</class>
    <class kind="struct">logging::Info</class>
    <class kind="struct">logging::Trace</class>
    <class kind="struct">logging::Debug</class>
    <class kind="class">logging::NullOutput</class>
    <class kind="class">logging::OutputLevelRunTimeSwitch</class>
    <class kind="class">logging::OutputLevelSwitchDisabled</class>
    <class kind="class">logging::OutputStream</class>
    <class kind="struct">logging::ProgramMemoryString</class>
    <class kind="class">logging::StdOutput</class>
  </compound>
  <compound kind="class">
    <name>logging::FileOutput</name>
    <filename>classlogging_1_1FileOutput.shtml</filename>
    <member kind="function">
      <type></type>
      <name>FileOutput</name>
      <anchorfile>classlogging_1_1FileOutput.shtml</anchorfile>
      <anchor>a503236165ed302cb809d29dacf0113b8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~FileOutput</name>
      <anchorfile>classlogging_1_1FileOutput.shtml</anchorfile>
      <anchor>a2d608d3ac0aa60de7f783ed208b9674b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>FileOutput &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1FileOutput.shtml</anchorfile>
      <anchor>a87b4da79a2651e1c3caf3400ab8af30d</anchor>
      <arglist>(const char c)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::log</name>
    <filename>structlogging_1_1log.shtml</filename>
    <member kind="enumeration">
      <type></type>
      <name>Numerative</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>bin</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9a79d46cee5eb4a6752720faac9213d64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>oct</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9aa79243a90870a6381b6ebdbceb6fd0c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>dec</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9aa4581b125a9a58e3ded6550e9cac14a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>hex</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9a67a26cbaa81611a99165c14aa43a67cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>bin</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9a79d46cee5eb4a6752720faac9213d64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>oct</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9aa79243a90870a6381b6ebdbceb6fd0c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>dec</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9aa4581b125a9a58e3ded6550e9cac14a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>hex</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a34625c57cc3eb8330503eea5c0e80fe9a67a26cbaa81611a99165c14aa43a67cb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Manipulator</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>acdd7dbf2da7c93960bcbcc5224338440</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>tab</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>acdd7dbf2da7c93960bcbcc5224338440a50bcef526b23979e002d6eae07c1db43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>endl</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>acdd7dbf2da7c93960bcbcc5224338440a830ddd59b5225652e0e61790cebecf22</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>tab</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>acdd7dbf2da7c93960bcbcc5224338440a50bcef526b23979e002d6eae07c1db43</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>endl</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>acdd7dbf2da7c93960bcbcc5224338440a830ddd59b5225652e0e61790cebecf22</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static typename::logging::detail::Logger&lt; Level &gt;::return_type &amp;</type>
      <name>emit</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>a011959e8306d83a9e8d1fd5095959d4b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static inline::logging::detail::Logger::return_type &amp;</type>
      <name>emit</name>
      <anchorfile>structlogging_1_1log.shtml</anchorfile>
      <anchor>afd0b58ce7a4fd78dadd0bc09f95c7a0b</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Level</name>
    <filename>structlogging_1_1Level.shtml</filename>
    <member kind="enumeration">
      <type></type>
      <name>levels</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>disable</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a92f866a946bc4cd84328d2255a42b104</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>error</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a7b1783711d2db165529702b46c209897</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>warning</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a6f7c481bbb7b74f7c2585bc1c4269007</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>normal</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a8f45128973efcb53bb1403bde028a81c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>info</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4afc30a65a97e34f7d22c1d7f4864d92e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>trace</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a73dd01e78b1d4256ad4a12ea2d747f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>debug</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a1be517598f6713bca1c819011691c38d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>user</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4afdd024043f29423f26f3737e8e9d20eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>disable</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a92f866a946bc4cd84328d2255a42b104</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>error</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a7b1783711d2db165529702b46c209897</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>warning</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a6f7c481bbb7b74f7c2585bc1c4269007</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>normal</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a8f45128973efcb53bb1403bde028a81c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>info</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4afc30a65a97e34f7d22c1d7f4864d92e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>trace</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a73dd01e78b1d4256ad4a12ea2d747f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>debug</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4a1be517598f6713bca1c819011691c38d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>user</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ad8e481eaf928970cf4e5760909a4ffd4afdd024043f29423f26f3737e8e9d20eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&amp;</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>ac5ea3ea07943746b2bd4b856309db6db</anchor>
      <arglist>(Level &amp;r)</arglist>
    </member>
    <member kind="function">
      <type>Level &amp;</type>
      <name>operator=</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>a9ceaaa0bae42b6f037f407a2dc9697d7</anchor>
      <arglist>(levels b)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>l</name>
      <anchorfile>structlogging_1_1Level.shtml</anchorfile>
      <anchor>a76f4033e39b06fb5415e4960b2919b22</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::RunTimeLevel</name>
    <filename>structlogging_1_1RunTimeLevel.shtml</filename>
    <base>logging::Level</base>
  </compound>
  <compound kind="struct">
    <name>logging::Error</name>
    <filename>structlogging_1_1Error.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Error.shtml</anchorfile>
      <anchor>a4648f76712009bfbcab4d9ec78aa8a65</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PROGMEMSTRINGTYPE</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Error.shtml</anchorfile>
      <anchor>abd3db1b30d0e27955e427c4bd804a49b</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Warning</name>
    <filename>structlogging_1_1Warning.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Warning.shtml</anchorfile>
      <anchor>a725fa99b3f561597ed16ec82b2a846f6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PROGMEMSTRINGTYPE</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Warning.shtml</anchorfile>
      <anchor>a09df1486899a0c9739d28cd072315ecc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Void</name>
    <filename>structlogging_1_1Void.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Void.shtml</anchorfile>
      <anchor>ae77955f2d4912c1960ea5a10088efb0e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const char *</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Void.shtml</anchorfile>
      <anchor>a01c11ce0a7a83ffee95bfae1686f5c76</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Info</name>
    <filename>structlogging_1_1Info.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Info.shtml</anchorfile>
      <anchor>aa0bfbcbcf810f38f6b1ab1ac4f61ddbe</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PROGMEMSTRINGTYPE</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Info.shtml</anchorfile>
      <anchor>a19e8c96c9311a51e03b2a8ac9a9a972a</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Trace</name>
    <filename>structlogging_1_1Trace.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Trace.shtml</anchorfile>
      <anchor>a49307f3f161520a2d44553f9605f0ce9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PROGMEMSTRINGTYPE</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Trace.shtml</anchorfile>
      <anchor>abaa1200dc3322d6fef182c05eab81087</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::Debug</name>
    <filename>structlogging_1_1Debug.shtml</filename>
    <member kind="function" static="yes">
      <type>::logging::Level::levels</type>
      <name>level</name>
      <anchorfile>structlogging_1_1Debug.shtml</anchorfile>
      <anchor>ae5e5cba83ae5280e628574f77f1ecd64</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PROGMEMSTRINGTYPE</type>
      <name>desc</name>
      <anchorfile>structlogging_1_1Debug.shtml</anchorfile>
      <anchor>af21e2a0c33bdde6e1a0c0599c0295db6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>logging::NullOutput</name>
    <filename>classlogging_1_1NullOutput.shtml</filename>
  </compound>
  <compound kind="class">
    <name>logging::OutputLevelRunTimeSwitch</name>
    <filename>classlogging_1_1OutputLevelRunTimeSwitch.shtml</filename>
    <templarg></templarg>
    <member kind="function">
      <type></type>
      <name>OutputLevelRunTimeSwitch</name>
      <anchorfile>classlogging_1_1OutputLevelRunTimeSwitch.shtml</anchorfile>
      <anchor>a9652c50819b7be7e89dd1b4e544e22f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>allowed</name>
      <anchorfile>classlogging_1_1OutputLevelRunTimeSwitch.shtml</anchorfile>
      <anchor>ac95bb4f7000c9686761cf19dd734678a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>OutputLevelRunTimeSwitch &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelRunTimeSwitch.shtml</anchorfile>
      <anchor>ad41a2352c458826c3c7ed7e591e96840</anchor>
      <arglist>(const ::logging::Level::levels &amp;l)</arglist>
    </member>
    <member kind="function">
      <type>OutputLevelRunTimeSwitch &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelRunTimeSwitch.shtml</anchorfile>
      <anchor>aefc1d49f32d6ed1b02f2c4235921dedd</anchor>
      <arglist>(const ::logging::RunTimeLevel &amp;l)</arglist>
    </member>
    <member kind="function">
      <type>OutputLevelRunTimeSwitch &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelRunTimeSwitch.shtml</anchorfile>
      <anchor>addd44a780b063ccc27ab27e06c202b8d</anchor>
      <arglist>(T t)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>logging::OutputLevelSwitchDisabled</name>
    <filename>classlogging_1_1OutputLevelSwitchDisabled.shtml</filename>
    <templarg></templarg>
    <member kind="function">
      <type>OutputLevelSwitchDisabled &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelSwitchDisabled.shtml</anchorfile>
      <anchor>a34b37fe2cddf049c5e0d8264b98b2e0d</anchor>
      <arglist>(const ::logging::Level::levels &amp;)</arglist>
    </member>
    <member kind="function">
      <type>OutputLevelSwitchDisabled &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelSwitchDisabled.shtml</anchorfile>
      <anchor>a7679405bdea75c76edbc392e1a2b47f5</anchor>
      <arglist>(const ::logging::RunTimeLevel &amp;l)</arglist>
    </member>
    <member kind="function">
      <type>OutputLevelSwitchDisabled &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputLevelSwitchDisabled.shtml</anchorfile>
      <anchor>aeefd3e381a63745c7f447aaf610a1c6e</anchor>
      <arglist>(T t)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>logging::OutputStream</name>
    <filename>classlogging_1_1OutputStream.shtml</filename>
    <templarg></templarg>
    <member kind="function">
      <type></type>
      <name>OutputStream</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>aa9d0224ce66debcb8bb15924c04f23c5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a60373343f6f4c56e264daa7568b9855a</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>acf3627860b02169f25c10e1a0db9ac3a</anchor>
      <arglist>(unsigned char c)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a67244f2018047a2fb834962491f66fce</anchor>
      <arglist>(const ::logging::log::Numerative n)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a463eebe508c2efdd7b243e48c0c8e455</anchor>
      <arglist>(const ::logging::log::Manipulator m)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a64ec0025b8219c1f8188937c722773ce</anchor>
      <arglist>(PROGMEMSTRINGTYPE &amp;pms)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a723a33805af924a01f514b2c0887b896</anchor>
      <arglist>(char *string)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>afcc5564654deaba6a3050a65e490c6c4</anchor>
      <arglist>(const char *string)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>ad5c29d740f8abfb15b593426a6df3c08</anchor>
      <arglist>(unsigned char *string)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a603b155e908ee9f8fe7670b2670638e3</anchor>
      <arglist>(const unsigned char *string)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>acbc701bb77838a0f0e00dedfb9447330</anchor>
      <arglist>(short ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a03a210fd57d5780aa06dff7dc6842d1d</anchor>
      <arglist>(unsigned short ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a3a050d9956da801f0358f352738279f6</anchor>
      <arglist>(int ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>ab104d5c99f175584341c1913c7e9dd7e</anchor>
      <arglist>(unsigned int ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a815fab9300d3d9bcd3772615ded427e5</anchor>
      <arglist>(long ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a17f46fd8cd3479eca09757dc6ca8ed76</anchor>
      <arglist>(unsigned long ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>ab88066ae2a8c10b7ea7063718a902d9d</anchor>
      <arglist>(long long ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>aa2ff77f858098ce7625fade450bfb53f</anchor>
      <arglist>(unsigned long long ival)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a264cc4c3588b34e1f31481ce25d10a2e</anchor>
      <arglist>(void *ptr)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a596c810d3f4399c857db8a07c92a5358</anchor>
      <arglist>(const void *ptr)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a61de046ba93bae56666acaa34e068e51</anchor>
      <arglist>(T)</arglist>
    </member>
    <member kind="function">
      <type>OutputStream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>ae737e814dd0de2780d594b3dd6a1988b</anchor>
      <arglist>(OutputStream &amp;(*f)(OutputStream &amp;))</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>unsigned char</type>
      <name>getBase</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>a4db175ccb4916dbc6314b1f856c68611</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>setBase</name>
      <anchorfile>classlogging_1_1OutputStream.shtml</anchorfile>
      <anchor>ad5277b1495ad7334ec5cc4c57ac69449</anchor>
      <arglist>(unsigned char tmp)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::ProgramMemoryString</name>
    <filename>structlogging_1_1ProgramMemoryString.shtml</filename>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>structlogging_1_1ProgramMemoryString.shtml</anchorfile>
      <anchor>abdb2832262db2e296d97cb671dfaf9c6</anchor>
      <arglist>(int index) const </arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator*</name>
      <anchorfile>structlogging_1_1ProgramMemoryString.shtml</anchorfile>
      <anchor>ab4deed7afc2164a59ccf1442d9fd15cc</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const ProgramMemoryString &amp;</type>
      <name>operator++</name>
      <anchorfile>structlogging_1_1ProgramMemoryString.shtml</anchorfile>
      <anchor>aeecf29fad50778a029a697f8a203a581</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ProgramMemoryString</type>
      <name>operator++</name>
      <anchorfile>structlogging_1_1ProgramMemoryString.shtml</anchorfile>
      <anchor>a2ee9565487076ad4b8a8e588b1b2f0af</anchor>
      <arglist>(int) const </arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>str</name>
      <anchorfile>structlogging_1_1ProgramMemoryString.shtml</anchorfile>
      <anchor>a7fe3db16be45eb5a53bfb8868d8e1be2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>logging::StdOutput</name>
    <filename>classlogging_1_1StdOutput.shtml</filename>
    <templarg>stream</templarg>
    <member kind="function">
      <type>StdOutput &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classlogging_1_1StdOutput.shtml</anchorfile>
      <anchor>af054a7389b9d2f2831d26864be187df8</anchor>
      <arglist>(const char c)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>logging::config</name>
    <filename>namespacelogging_1_1config.shtml</filename>
  </compound>
  <compound kind="namespace">
    <name>logging::detail</name>
    <filename>namespacelogging_1_1detail.shtml</filename>
    <class kind="class">logging::detail::Logger</class>
    <class kind="struct">logging::detail::singleton</class>
  </compound>
  <compound kind="class">
    <name>logging::detail::Logger</name>
    <filename>classlogging_1_1detail_1_1Logger.shtml</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="typedef">
      <type>R</type>
      <name>return_type</name>
      <anchorfile>classlogging_1_1detail_1_1Logger.shtml</anchorfile>
      <anchor>a5845c7d5f11903d89802ffca90da5e51</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static return_type &amp;</type>
      <name>logging</name>
      <anchorfile>classlogging_1_1detail_1_1Logger.shtml</anchorfile>
      <anchor>abb8e1cb369fe6778d64d6673385f86fc</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>logging::detail::singleton</name>
    <filename>structlogging_1_1detail_1_1singleton.shtml</filename>
    <templarg></templarg>
    <member kind="typedef">
      <type>T</type>
      <name>obj_type</name>
      <anchorfile>structlogging_1_1detail_1_1singleton.shtml</anchorfile>
      <anchor>a568fe910d6909af44e3c376c7b99dbf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static obj_type &amp;</type>
      <name>instance</name>
      <anchorfile>structlogging_1_1detail_1_1singleton.shtml</anchorfile>
      <anchor>a1af860ef634e2ee953da18b55f449880</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>include</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/</path>
    <filename>dir_d44c64559bbebec7f509842c48db8b23.shtml</filename>
    <dir>include/logging</dir>
  </compound>
  <compound kind="dir">
    <name>include/logging</name>
    <path>/Users/andreas/code/AVR-HaLib/externals/logging-cpp-svn/include/logging/</path>
    <filename>dir_7188461832bb92671066220d237ea5f7.shtml</filename>
    <file>FileOutput.h</file>
    <file>Logger.h</file>
    <file>LoggerLevel.h</file>
    <file>logging.h</file>
    <file>loggingConfigAVR.h</file>
    <file>loggingConfigGeneralPurposeOS.h</file>
    <file>NullOutput.h</file>
    <file>OutputLevelRunTimeSwitch.h</file>
    <file>OutputLevelSwitchDisabled.h</file>
    <file>OutputStream.h</file>
    <file>ProgramMemoryString.h</file>
    <file>ProgramMemoryStringImpl.h</file>
    <file>singleton.h</file>
    <file>StdOutput.h</file>
  </compound>
</tagfile>
