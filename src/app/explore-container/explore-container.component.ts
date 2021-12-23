import { HttpClient } from '@angular/common/http';
import { Component, OnInit, Input } from '@angular/core';
import { Subscription, take } from 'rxjs';
import { Storage } from '@capacitor/storage';

@Component({
  selector: 'app-explore-container',
  templateUrl: './explore-container.component.html',
  styleUrls: ['./explore-container.component.scss'],
})
export class ExploreContainerComponent implements OnInit {
  @Input() name: string;
  @Input() ip: string;
  public steps = 6000;
  public rotations = 3;
  public speed = 500;
  public accel = 200;
  public messages: any[] = [];
  public stepsDisabled: boolean = true;
  public rotationsDisabled: boolean = false;
  constructor(private httpClient: HttpClient) {
    this.httpClient.get(`http://${this.ip}/`, { responseType: 'text' }).pipe(take(1)).subscribe(x => {

      this.messages.splice(0, 0, x);
    });
  }

  public changeSpeed() {
    if (this.accel > this.speed) {
      this.accel = this.speed;
    }
  }

  public init() {
    let message = `http://${this.ip}/Init?Delay=${this.speed}&Steps=${this.steps}&Accel=${this.accel}`;
    if (this.stepsDisabled) {
      message = `http://${this.ip}/Init?&Delay=${this.speed}&Rotations=${this.rotations}&Accel=${this.accel}`;
    }
    this.httpClient.get(message, { responseType: 'text' }).pipe(take(1)).subscribe(x => {
      this.messages.splice(0, 0, x);
    });
  }

  public stop() {
    this.httpClient.get(`http://${this.ip}/Stop`, { responseType: 'text' }).pipe(take(1)).subscribe(x => {
      this.messages.splice(0, 0, x);
    });
  }

  public toggle(data) {
    console.log(data);
    if (!this.stepsDisabled) {
      this.stepsDisabled = true;
      this.rotationsDisabled = false;
    } else {
      this.stepsDisabled = false;
      this.rotationsDisabled = true;
    }

  }

  public toggleStepsDisabled() {
    this.stepsDisabled = false;
    this.rotationsDisabled = true;
  }

  public toggleRotationsDisabled() {
    this.stepsDisabled = true;
    this.rotationsDisabled = false;
  }

  ngOnInit() { }

}


