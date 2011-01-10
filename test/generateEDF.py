



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<!-- ViewVC :: http://www.viewvc.org/ -->
<head>
<title>[CMSSW] Log of /CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py</title>
<meta name="generator" content="ViewVC 1.0.4" />
<link rel="stylesheet" href="/cgi-bin/cmssw.cgi/*docroot*/styles.css" type="text/css" />

</head>
<body>
<div class="vc_navheader">

<form method="get" action="/cgi-bin/cmssw.cgi/">

<table style="padding:0.1em;">
<tr>
<td>
<strong>

<a href="/cgi-bin/cmssw.cgi/">

[CMSSW]</a>
/

<a href="/cgi-bin/cmssw.cgi/CMSSW/">

CMSSW</a>
/

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/">

FWCore</a>
/

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/">

PythonUtilities</a>
/

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/">

scripts</a>
/



generateEDF.py


</strong>

</td>
<td style="text-align:right;">


<strong>Repository:</strong>
<select name="root" onchange="submit()">


<option value="*viewroots*">Repository Listing</option>










<optgroup label="CVS Repositories"><option selected="selected">CMSSW</option></optgroup>


</select>
<input type="submit" value="Go" />

</td>
</tr>
</table>

</form>

</div>
<div style="float: right; padding: 5px;"><a href="http://www.viewvc.org/"><img src="/cgi-bin/cmssw.cgi/*docroot*/images/logo.png" alt="ViewVC logotype" width="128" height="48" /></a></div>
<h1>Log of /CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py</h1>

<p style="margin:0;">

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/"><img src="/cgi-bin/cmssw.cgi/*docroot*/images/back_small.png" width="16" height="16" alt="Parent Directory" /> Parent Directory</a>


| <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=graph"><img src="/cgi-bin/cmssw.cgi/*docroot*/images/cvsgraph_16x16.png" width="16" height="16" alt="View Revision Graph" /> Revision Graph</a>



</p>

<hr />
<table class="auto">



<tr>
<td>Links to HEAD:</td>
<td>
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=markup">view</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=co">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?content-type=text%2Fplain&amp;view=co">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=annotate">annotate</a>)
</td>
</tr>



<tr>
<td>Sticky Tag:</td>
<td><form method="get" action="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py" style="display: inline">
<div style="display: inline">
<input type="hidden" name="view" value="log" />


<select name="pathrev" onchange="submit()">
<option value=""></option>

<optgroup label="Branches">


<option>MAIN</option>


</optgroup>

<optgroup label="Non-branch tags">


<option>V01-07-00</option>



<option>V01-06-09</option>



<option>V01-06-08</option>



<option>V01-06-07</option>



<option>V01-06-06</option>



<option>V01-06-05</option>



<option>V01-06-04</option>



<option>V01-06-03</option>



<option>V01-06-02</option>



<option>V01-06-01</option>



<option>V01-06-00</option>



<option>HEAD</option>



<option>CMSSW_3_9_2_patch5</option>



<option>CMSSW_3_9_2_patch4</option>



<option>CMSSW_3_9_2_patch3</option>



<option>CMSSW_3_9_2_patch2</option>



<option>CMSSW_3_9_2_patch1</option>



<option>CMSSW_3_9_2</option>



<option>CMSSW_3_9_1_patch1</option>



<option>CMSSW_3_9_1</option>



<option>CMSSW_3_9_0_pre7</option>



<option>CMSSW_3_9_0_pre6</option>



<option>CMSSW_3_9_0_pre5</option>



<option>CMSSW_3_9_0_pre4</option>



<option>CMSSW_3_9_0</option>



<option>CMSSW_3_10_0_pre4</option>



<option>CMSSW_3_10_0_pre3</option>



<option>CMSSW_3_10_0_pre2</option>



<option>CMSSW_3_10_0_pre1</option>


</optgroup>

</select>

<input type="submit" value="Set" />
</div>
</form>

</td>
</tr>
</table>
 








<div>
<hr />

<a name="rev1.9"></a>
<a name="V01-06-07"></a>
<a name="V01-06-08"></a>
<a name="V01-06-09"></a>
<a name="V01-07-00"></a>
<a name="CMSSW_3_9_1_patch1"></a>
<a name="CMSSW_3_10_0_pre3"></a>
<a name="CMSSW_3_10_0_pre2"></a>
<a name="CMSSW_3_10_0_pre1"></a>
<a name="CMSSW_3_10_0_pre4"></a>
<a name="CMSSW_3_9_1"></a>
<a name="CMSSW_3_9_0"></a>
<a name="CMSSW_3_9_2"></a>
<a name="CMSSW_3_9_2_patch3"></a>
<a name="CMSSW_3_9_2_patch2"></a>
<a name="CMSSW_3_9_2_patch1"></a>
<a name="CMSSW_3_9_2_patch5"></a>
<a name="CMSSW_3_9_2_patch4"></a>
<a name="CMSSW_3_9_0_pre5"></a>
<a name="CMSSW_3_9_0_pre6"></a>
<a name="CMSSW_3_9_0_pre7"></a>
<a name="HEAD"></a>


Revision <strong>1.9</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.9&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.9">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.9&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.9">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.9&amp;view=log">[select for diffs]</a>




<br />

<em>Wed Sep 15 18:22:49 2010 UTC</em> (8 weeks, 2 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_10_0_pre1"><strong>CMSSW_3_10_0_pre1</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_10_0_pre2"><strong>CMSSW_3_10_0_pre2</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_10_0_pre3"><strong>CMSSW_3_10_0_pre3</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_10_0_pre4"><strong>CMSSW_3_10_0_pre4</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_0"><strong>CMSSW_3_9_0</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_0_pre5"><strong>CMSSW_3_9_0_pre5</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_0_pre6"><strong>CMSSW_3_9_0_pre6</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_0_pre7"><strong>CMSSW_3_9_0_pre7</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_1"><strong>CMSSW_3_9_1</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_1_patch1"><strong>CMSSW_3_9_1_patch1</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2"><strong>CMSSW_3_9_2</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2_patch1"><strong>CMSSW_3_9_2_patch1</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2_patch2"><strong>CMSSW_3_9_2_patch2</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2_patch3"><strong>CMSSW_3_9_2_patch3</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2_patch4"><strong>CMSSW_3_9_2_patch4</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_2_patch5"><strong>CMSSW_3_9_2_patch5</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=HEAD"><strong>HEAD</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-07"><strong>V01-06-07</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-08"><strong>V01-06-08</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-09"><strong>V01-06-09</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-07-00"><strong>V01-07-00</strong></a>






<br />Changes since <strong>1.8: +17 -6 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.8&amp;r2=1.9">previous 1.8</a>










<pre class="vc_log">fixed bug in instLum modes introduced with '--breakExpectedIntLum'; added -noDataPoints option
</pre>
</div>



<div>
<hr />

<a name="rev1.8"></a>
<a name="V01-06-06"></a>


Revision <strong>1.8</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.8&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.8">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.8&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.8">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.8&amp;view=log">[select for diffs]</a>




<br />

<em>Tue Sep 14 16:13:02 2010 UTC</em> (8 weeks, 3 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-06"><strong>V01-06-06</strong></a>






<br />Changes since <strong>1.7: +166 -54 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.7&amp;r2=1.8">previous 1.7</a>










<pre class="vc_log">added --breakExpectedIntLum
</pre>
</div>



<div>
<hr />

<a name="rev1.7"></a>
<a name="V01-06-05"></a>


Revision <strong>1.7</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.7&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.7">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.7&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.7">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.7&amp;view=log">[select for diffs]</a>




<br />

<em>Tue Sep 14 01:20:54 2010 UTC</em> (8 weeks, 3 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-05"><strong>V01-06-05</strong></a>






<br />Changes since <strong>1.6: +14 -3 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.6&amp;r2=1.7">previous 1.6</a>










<pre class="vc_log">added --minIntLum and --maxIntLum
</pre>
</div>



<div>
<hr />

<a name="rev1.6"></a>


Revision <strong>1.6</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.6&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.6">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.6&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.6">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.6&amp;view=log">[select for diffs]</a>




<br />

<em>Tue Sep 14 01:12:48 2010 UTC</em> (8 weeks, 3 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>







<br />Changes since <strong>1.5: +33 -41 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.5&amp;r2=1.6">previous 1.5</a>










<pre class="vc_log">better handling of minRun and maxRun
</pre>
</div>



<div>
<hr />

<a name="rev1.5"></a>
<a name="V01-06-04"></a>


Revision <strong>1.5</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.5&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.5">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.5&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.5">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.5&amp;view=log">[select for diffs]</a>




<br />

<em>Mon Sep 13 20:56:33 2010 UTC</em> (8 weeks, 3 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-04"><strong>V01-06-04</strong></a>






<br />Changes since <strong>1.4: +4 -3 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.4&amp;r2=1.5">previous 1.4</a>










<pre class="vc_log">fixed spacing
</pre>
</div>



<div>
<hr />

<a name="rev1.4"></a>


Revision <strong>1.4</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.4&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.4">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.4&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.4">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.4&amp;view=log">[select for diffs]</a>




<br />

<em>Mon Sep 13 20:53:05 2010 UTC</em> (8 weeks, 3 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>







<br />Changes since <strong>1.3: +27 -4 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.3&amp;r2=1.4">previous 1.3</a>










<pre class="vc_log">starting luminosity value is consistent with --minRun is used (i.e., not 0, but has same value it would have been)
</pre>
</div>



<div>
<hr />

<a name="rev1.3"></a>
<a name="V01-06-03"></a>


Revision <strong>1.3</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.3&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.3">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.3&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.3">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.3&amp;view=log">[select for diffs]</a>




<br />

<em>Mon Sep 13 19:51:15 2010 UTC</em> (8 weeks, 4 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-03"><strong>V01-06-03</strong></a>






<br />Changes since <strong>1.2: +15 -0 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.2&amp;r2=1.3">previous 1.2</a>










<pre class="vc_log">added --minRun and --maxRun options
</pre>
</div>



<div>
<hr />

<a name="rev1.2"></a>
<a name="V01-06-02"></a>
<a name="V01-06-01"></a>


Revision <strong>1.2</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.2&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.2">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.2&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.2">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.2&amp;view=log">[select for diffs]</a>




<br />

<em>Mon Sep 13 15:11:57 2010 UTC</em> (8 weeks, 4 days ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-01"><strong>V01-06-01</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-02"><strong>V01-06-02</strong></a>






<br />Changes since <strong>1.1: +3 -3 lines</strong>







<br />Diff to <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.1&amp;r2=1.2">previous 1.1</a>










<pre class="vc_log">fixed bug in which lumi unit is chosen
</pre>
</div>



<div>
<hr />

<a name="rev1.1"></a>
<a name="V01-06-00"></a>
<a name="CMSSW_3_9_0_pre4"></a>


Revision <strong>1.1</strong> -

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.1&amp;view=markup">view</a>)

(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.1">download</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?revision=1.1&amp;content-type=text%2Fplain">as text</a>)
(<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?annotate=1.1">annotate</a>)



- <a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?r1=1.1&amp;view=log">[select for diffs]</a>




<br />

<em>Wed Sep  1 17:19:33 2010 UTC</em> (2 months, 1 week ago) by <em>cplager</em>


<br />Branch:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=MAIN"><strong>MAIN</strong></a>



<br />CVS Tags:

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=CMSSW_3_9_0_pre4"><strong>CMSSW_3_9_0_pre4</strong></a>,

<a href="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py?view=log&amp;pathrev=V01-06-00"><strong>V01-06-00</strong></a>















<pre class="vc_log">script that generates empirical distribution functions
</pre>
</div>

 



 <hr />
<p><a name="diff"></a>
This form allows you to request diffs between any two revisions of this file.
For each of the two "sides" of the diff,

select a symbolic revision name using the selection box, or choose
'Use Text Field' and enter a numeric revision.

</p>
<form method="get" action="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py" id="diff_select">
<table cellpadding="2" cellspacing="0" class="auto">
<tr>
<td>&nbsp;</td>
<td>
<input type="hidden" name="view" value="diff" />
Diffs between

<select name="r1">
<option value="text" selected="selected">Use Text Field</option>

<option value="1.9:V01-07-00">V01-07-00</option>

<option value="1.9:V01-06-09">V01-06-09</option>

<option value="1.9:V01-06-08">V01-06-08</option>

<option value="1.9:V01-06-07">V01-06-07</option>

<option value="1.8:V01-06-06">V01-06-06</option>

<option value="1.7:V01-06-05">V01-06-05</option>

<option value="1.5:V01-06-04">V01-06-04</option>

<option value="1.3:V01-06-03">V01-06-03</option>

<option value="1.2:V01-06-02">V01-06-02</option>

<option value="1.2:V01-06-01">V01-06-01</option>

<option value="1.1:V01-06-00">V01-06-00</option>

<option value="1.9:MAIN">MAIN</option>

<option value="1.9:HEAD">HEAD</option>

<option value="1.9:CMSSW_3_9_2_patch5">CMSSW_3_9_2_patch5</option>

<option value="1.9:CMSSW_3_9_2_patch4">CMSSW_3_9_2_patch4</option>

<option value="1.9:CMSSW_3_9_2_patch3">CMSSW_3_9_2_patch3</option>

<option value="1.9:CMSSW_3_9_2_patch2">CMSSW_3_9_2_patch2</option>

<option value="1.9:CMSSW_3_9_2_patch1">CMSSW_3_9_2_patch1</option>

<option value="1.9:CMSSW_3_9_2">CMSSW_3_9_2</option>

<option value="1.9:CMSSW_3_9_1_patch1">CMSSW_3_9_1_patch1</option>

<option value="1.9:CMSSW_3_9_1">CMSSW_3_9_1</option>

<option value="1.9:CMSSW_3_9_0_pre7">CMSSW_3_9_0_pre7</option>

<option value="1.9:CMSSW_3_9_0_pre6">CMSSW_3_9_0_pre6</option>

<option value="1.9:CMSSW_3_9_0_pre5">CMSSW_3_9_0_pre5</option>

<option value="1.1:CMSSW_3_9_0_pre4">CMSSW_3_9_0_pre4</option>

<option value="1.9:CMSSW_3_9_0">CMSSW_3_9_0</option>

<option value="1.9:CMSSW_3_10_0_pre4">CMSSW_3_10_0_pre4</option>

<option value="1.9:CMSSW_3_10_0_pre3">CMSSW_3_10_0_pre3</option>

<option value="1.9:CMSSW_3_10_0_pre2">CMSSW_3_10_0_pre2</option>

<option value="1.9:CMSSW_3_10_0_pre1">CMSSW_3_10_0_pre1</option>

</select>
<input type="text" size="12" name="tr1"
value="1.9"
onchange="document.getElementById('diff_select').r1.selectedIndex=0" />

and

<select name="r2">
<option value="text" selected="selected">Use Text Field</option>

<option value="1.9:V01-07-00">V01-07-00</option>

<option value="1.9:V01-06-09">V01-06-09</option>

<option value="1.9:V01-06-08">V01-06-08</option>

<option value="1.9:V01-06-07">V01-06-07</option>

<option value="1.8:V01-06-06">V01-06-06</option>

<option value="1.7:V01-06-05">V01-06-05</option>

<option value="1.5:V01-06-04">V01-06-04</option>

<option value="1.3:V01-06-03">V01-06-03</option>

<option value="1.2:V01-06-02">V01-06-02</option>

<option value="1.2:V01-06-01">V01-06-01</option>

<option value="1.1:V01-06-00">V01-06-00</option>

<option value="1.9:MAIN">MAIN</option>

<option value="1.9:HEAD">HEAD</option>

<option value="1.9:CMSSW_3_9_2_patch5">CMSSW_3_9_2_patch5</option>

<option value="1.9:CMSSW_3_9_2_patch4">CMSSW_3_9_2_patch4</option>

<option value="1.9:CMSSW_3_9_2_patch3">CMSSW_3_9_2_patch3</option>

<option value="1.9:CMSSW_3_9_2_patch2">CMSSW_3_9_2_patch2</option>

<option value="1.9:CMSSW_3_9_2_patch1">CMSSW_3_9_2_patch1</option>

<option value="1.9:CMSSW_3_9_2">CMSSW_3_9_2</option>

<option value="1.9:CMSSW_3_9_1_patch1">CMSSW_3_9_1_patch1</option>

<option value="1.9:CMSSW_3_9_1">CMSSW_3_9_1</option>

<option value="1.9:CMSSW_3_9_0_pre7">CMSSW_3_9_0_pre7</option>

<option value="1.9:CMSSW_3_9_0_pre6">CMSSW_3_9_0_pre6</option>

<option value="1.9:CMSSW_3_9_0_pre5">CMSSW_3_9_0_pre5</option>

<option value="1.1:CMSSW_3_9_0_pre4">CMSSW_3_9_0_pre4</option>

<option value="1.9:CMSSW_3_9_0">CMSSW_3_9_0</option>

<option value="1.9:CMSSW_3_10_0_pre4">CMSSW_3_10_0_pre4</option>

<option value="1.9:CMSSW_3_10_0_pre3">CMSSW_3_10_0_pre3</option>

<option value="1.9:CMSSW_3_10_0_pre2">CMSSW_3_10_0_pre2</option>

<option value="1.9:CMSSW_3_10_0_pre1">CMSSW_3_10_0_pre1</option>

</select>
<input type="text" size="12" name="tr2"
value="1.1"
onchange="document.getElementById('diff_select').r2.selectedIndex=0" />

</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
Type of Diff should be a
<select name="diff_format" onchange="submit()">
<option value="h" selected="selected">Colored Diff</option>
<option value="l" >Long Colored Diff</option>
<option value="u" >Unidiff</option>
<option value="c" >Context Diff</option>
<option value="s" >Side by Side</option>
</select>
<input type="submit" value=" Get Diffs " />
</td>
</tr>
</table>
</form>


<form method="get" action="/cgi-bin/cmssw.cgi/CMSSW/FWCore/PythonUtilities/scripts/generateEDF.py">
<div>
<hr />
<a name="logsort"></a>
<input type="hidden" name="view" value="log" />
Sort log by:
<select name="logsort" onchange="submit()">
<option value="cvs" >Not sorted</option>
<option value="date" selected="selected">Commit date</option>
<option value="rev" >Revision</option>
</select>
<input type="submit" value=" Sort " />
</div>
</form>


<hr />
<table>
<tr>
<td><address><a href="http://lcgcvs.web.cern.ch/lcgcvs">CERN LCG CVS service</a></address></td>
<td style="text-align: right;"><strong><a href="/cgi-bin/cmssw.cgi/*docroot*/help_log.html">ViewVC Help</a></strong></td>
</tr>
<tr>
<td>Powered by <a href="http://viewvc.tigris.org/">ViewVC 1.0.4</a></td>
<td style="text-align: right;">&nbsp;</td>
</tr>
</table>
</body>
</html>


