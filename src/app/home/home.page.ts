import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { timeout } from 'rxjs/operators';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage {
  speed = 500;
  action = "stop";
  constructor(
    private http: HttpClient,
  ) {
    this.GetSpeed();
  }
  GetSend(act) {
    this.action = act;
    this.http.get("http://192.168.4.1/" + act + "?t=" + new Date().getTime())
      .pipe(
        timeout(1000)
      )
      .subscribe((response: any) => {
        this.action = response;
      }, error => {
      });
  }
  Speed() {
    this.http.get("http://192.168.4.1/speed?val=" + this.speed + "&t=" + new Date().getTime())
      .pipe(
        timeout(1000)
      )
      .subscribe((response: any) => {
      }, error => {
      });
  }
  GetSpeed() {
    this.http.get("http://192.168.4.1/speed?t=" + new Date().getTime())
      .pipe(
        timeout(1000)
      )
      .subscribe((response: any) => {
        this.speed = parseInt(response);
      }, error => {
      });
  }
}
