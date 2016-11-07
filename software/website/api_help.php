<!DOCTYPE html>
<title>API Help</title>
<p>You can use the following parameters in a querystring:</p>
<table>
  <thead>
    <th>Parameter</th>
    <th>Description</th>
  </thead>
  <tbody>
    <tr>
      <td>t</td>
      <td>Type of action</td>
    </tr>
    <tr>
      <td>d</td>
      <td>Device id</td>
    </tr>
    <tr>
      <td>td</td>
      <td>Target device id</td>
    </tr>
    <tr>
      <td>c</td>
      <td>Hex color</td>
    </tr>
  </tbody>
</table>
<p>Type of actions (t):</p>
<table>
  <thead>
    <th>Key</th>
    <th>Description</th>
    <th>Parameters</th>
  </thead>
  <tbody>
    <tr>
      <td>sdc</td>
      <td>Set device configuration</td>
      <td>d*, dt*, c*</td>
    </tr>
    <tr>
      <td>rdc</td>
      <td>Remove device configuration</td>
      <td>d*, dt*</td>
    </tr>
    <tr>
      <td>gqi</td>
      <td>Get query item</td>
      <td>d*</td>
    </tr>
    <tr>
      <td>sqi</td>
      <td>Set query item</td>
      <td>d*</td>
    </tr>
  </tbody>
  <tfoot>
    * is required
  </tfoot>
</table>
<p>Example of a querystring: ?t=sdc&d=T111&td=T222&c=ff0000, which sets a device configuration by T111 for T222 with the hex color ff0000</p>
